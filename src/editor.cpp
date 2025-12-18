#include "../include/editor.h"
#include "../include/bandit.h"
#include "../include/ork.h"
#include "../include/werewolf.h"
#include "../include/npc.h"
#include <fstream>
#include <ctime>
#include <string>
#include <vector>
#include <cstdlib>

class TextObserver : public IFightObserver
{
private:
    TextObserver(){};

public:
    static std::shared_ptr<IFightObserver> get()
    {
        static TextObserver instance;
        return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
    }

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override
    {
        if (win)
        {
            std::cout << "Murder --------" << std::endl;
            std::cout << "Attacker: ";
            attacker->print();
            std::cout << "Killed: ";
            defender->print();
            std::cout << std::endl;
        }
    }
};

class FileObserver : public IFightObserver
{
private:
    FileObserver(){};

public:
    static std::shared_ptr<IFightObserver> get()
    {
        static FileObserver instance;
        return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
    }

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override
    {
        if (win)
        {
            std::ofstream ofs("../log.txt", std::ios::app);
            ofs << "Murder --------" << std::endl;
            ofs << "Attacker: ";
            attacker->save(ofs);
            ofs << "Killed: ";
            defender->save(ofs);
            ofs << std::endl;
        }
    }
};

void initialize_editor() {
    std::ofstream file("../log.txt", std::ios::trunc);
}

int get_type_from_string(const std::string &str) {
    if (str == "bandit") {
        return BanditType;
    }
    if (str == "ork") {
        return OrkType;
    }
    if (str == "werewolf") {
        return WerewolfType;
    }
    return 0;
}

std::shared_ptr<NPC> factory(std::istream &is)
{
    std::shared_ptr<NPC> result;
    int type_int = 0;
    if (!(is >> type_int)) {
        std::cerr << "Error: failed to read NPC type from stream" << std::endl;
        return nullptr;
    }
    NpcType type = static_cast<NpcType>(type_int);

    switch (type)
    {
    case BanditType:
        result = std::make_shared<Bandit>(is);
        break;
    case OrkType:
        result = std::make_shared<Ork>(is);
        break;
    case WerewolfType:
        result = std::make_shared<Werewolf>(is);
        break;
    default:
        std::cerr << "unexpected NPC type: " << type_int << std::endl;
        break;
    }

    if (result) {
        result->subscribe(TextObserver::get());
        result->subscribe(FileObserver::get());
    }

    return result;
}

std::shared_ptr<NPC> factory(NpcType type, int x, int y)
{
    std::shared_ptr<NPC> result;
    switch (type)
    {
    case BanditType:
        result = std::make_shared<Bandit>(x, y);
        break;
    case OrkType:
        result = std::make_shared<Ork>(x, y);
        break;
    case WerewolfType:
        result = std::make_shared<Werewolf>(x, y);
        break;
    default:
        break;
    }
    if (result) {
        result->subscribe(TextObserver::get());
        result->subscribe(FileObserver::get());
    }

    std::cout << "Created ";
    result->print();

    return result;
}

void save(const set_t &array, const std::string &filename)
{
    std::ofstream fs(filename);
    fs << array.size() << std::endl;
    for (auto &n : array)
        n->save(fs);
    fs.flush();
    fs.close();
}

set_t load(const std::string &filename)
{
    set_t result;
    std::ifstream is(filename);
    if (is.good() && is.is_open())
    {
        int count = 0;
        is >> count;
        for (int i = 0; i < count; ++i) {
            auto npc = factory(is);
            if (npc) {
                result.insert(npc);
            }
        }
        is.close();
    }
    else
        std::cerr << "Error: " << std::strerror(errno) << std::endl;
    return result;
}

std::ostream &operator<<(std::ostream &os, const set_t &array)
{
    for (auto &n : array)
        n->print();
    return os;
}

set_t fight(const set_t &array, size_t distance)
{
    set_t dead_list;

    for (const auto &attacker : array)
        for (const auto &defender : array)
            if ((attacker != defender) && (attacker->is_close(defender, distance)))
            {
                if (defender->accept(attacker))
                    dead_list.insert(defender);
            }

    return dead_list;
}

void print_npcs(const set_t &array)
{
    std::cout << "NPCs:" << std::endl;
    for (const auto &n : array)
        n->print();
}

void simulate_fight(set_t &array, size_t max_fight_distance) {
    std::cout << std::endl;
    std::cout << "Simulating fights ..." << std::endl;
    int i;
    for (size_t distance = 20; (distance <= max_fight_distance) && !array.empty(); distance += 10)
    {
        std::cout << "Fighting at distance: " << distance << std::endl;
        auto dead_list = fight(array, distance);
        for (auto &d : dead_list)
            array.erase(d);
        std::cout << "Fight stats ----------" << std::endl
                  << "distance: " << distance << std::endl
                  << "killed: " << dead_list.size() << std::endl
                  << std::endl << std::endl;

    }
}

void generate_npcs(set_t &array, size_t count, size_t field_size) {
    std::srand(std::time(nullptr));
    for (size_t i = 0; i < count; ++i) {
        array.insert(factory(
            NpcType(std::rand() % 3 + 1),
            std::rand() % field_size,
            std::rand() % field_size
        ));
    }
}