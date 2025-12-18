#include "include/npc.h"
#include "include/editor.h"

int main()
{
    initialize_editor();

    set_t array;

    std::cout << "Generating ..." << std::endl;
    generate_npcs(array, 10, 501);

    std::cout << "Saving ..." << std::endl;
    save(array, "../npc.txt");

    std::cout << "Loading ..." << std::endl;
    array = load("../npc.txt");

    print_npcs(array);

    simulate_fight(array, 100);

    std::cout << "Survivors:\n" << array;

    return 0;
}