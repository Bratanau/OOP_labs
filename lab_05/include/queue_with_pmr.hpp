#pragma once

#include <memory_resource>
#include <iostream>
#include <list>
#include <algorithm>
#include <stdexcept>

// Простой ресурс памяти с фиксированным буфером
class FixedBlockMemoryResource : public std::pmr::memory_resource {
private:
    struct MemoryBlock {
        size_t offset{0}; // смещение 
        size_t size{0}; // размер блока
        MemoryBlock(size_t off, size_t sz) : offset(off), size(sz) {}
    };

    static constexpr size_t BUFFER_SIZE = 600;          // размер буфера
    char memory_buffer[BUFFER_SIZE]{};                  // статический буфер
    std::list<MemoryBlock> used_blocks;                 // занятые блоки

protected:
    void* do_allocate(size_t bytes, size_t alignment) override { // выделение памяти
        size_t allocation_offset = 0;

        for (const auto& block : used_blocks) {
            if (allocation_offset + bytes <= block.offset) {
                // достаточно места перед этим блоком
                break;
            }
            allocation_offset = block.offset + block.size;
        }

        if (allocation_offset + bytes > BUFFER_SIZE) {
            throw std::bad_alloc();
        }

        used_blocks.emplace_back(allocation_offset, bytes);
        return memory_buffer + allocation_offset;
    }

    void do_deallocate(void* p, size_t bytes, size_t alignment) override {
        auto it = std::find_if(used_blocks.begin(), used_blocks.end(),
            [&](const MemoryBlock& block) {
                return (memory_buffer + block.offset) == p && block.size == bytes;
            });

        if (it != used_blocks.end()) {
            used_blocks.erase(it);
        }
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }

public:
    ~FixedBlockMemoryResource() override {
        used_blocks.clear();
    }

    void print_memory_state() const {
        std::cout << "FixedBlockMemoryResource state:\n";
        std::cout << "  Buffer size: " << BUFFER_SIZE << " bytes\n";
        std::cout << "  Used blocks: " << used_blocks.size() << "\n";
        size_t used = 0;
        for (const auto& block : used_blocks) {
            used += block.size;
            std::cout << "    offset=" << block.offset
                      << ", size=" << block.size << "\n";
        }
        std::cout << "  Used bytes: " << used << "\n";
        std::cout << "  Free bytes: " << (BUFFER_SIZE - used) << "\n";
    }
};
//////////////////////////////////////////////////////
template<typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value, Node* next_node = nullptr)
            : data(value), next(next_node) {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    size_t queue_size = 0;

    std::pmr::polymorphic_allocator<Node> allocator;

public:
    using allocator_type = std::pmr::polymorphic_allocator<Node>;

    class Iterator {
        Node* current;
    public:
        Iterator(Node* node = nullptr) : current(node) {}

        T& operator*() const { return current->data; }
        Iterator& operator++() {
            current = current->next; 
            return *this; 
        }
        bool operator!=(const Iterator& other) const { 
            return current != other.current; 
        }
        bool operator==(const Iterator& other) const { 
            return current == other.current; 
        }
    };

    explicit Queue(std::pmr::memory_resource* res = std::pmr::get_default_resource())
        : allocator(res) {}

    ~Queue() { clear(); }

    // Добавление в конец очереди
    void push(const T& value) {
        Node* new_node = allocator.allocate(1);
        std::allocator_traits<allocator_type>::construct(allocator, new_node, value, nullptr);
        
        if (empty()) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        ++queue_size;
    }

    void pop() { // удаление из начала очереди
        if (empty()) return;

        Node* old_head = head;
        head = head->next;

        std::allocator_traits<allocator_type>::destroy(allocator, old_head);
        allocator.deallocate(old_head, 1);

        --queue_size;
        if (empty()) {
            tail = nullptr;
        }
    }

    // Доступ к первому элементу
    T& front() { 
        if (empty()) throw std::runtime_error("Queue is empty");
        return head->data;
    }

    const T& front() const {
        if (empty()) throw std::runtime_error("Queue is empty");
        return head->data;
    }

    // Доступ к последнему элементу
    T& back() { 
        if (empty()) throw std::runtime_error("Queue is empty");
        return tail->data; 
    }
    
    const T& back() const { 
        if (empty()) throw std::runtime_error("Queue is empty");
        return tail->data; 
    }

    bool empty() const { return head == nullptr; }
    size_t size() const { return queue_size; }

    void clear() {
        while (!empty()) pop();
    }

    // Итераторы
    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
};