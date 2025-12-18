#ifndef EDITOR__H
#define EDITOR__H
#include "npc.h"
#include <memory>


void initialize_editor();

std::shared_ptr<NPC> factory(std::istream &is);
std::shared_ptr<NPC> factory(NpcType type, int x, int y);

void save(const set_t &array, const std::string &filename);

set_t load(const std::string &filename);

std::ostream &operator<<(std::ostream &os, const set_t &array);

set_t fight(const set_t &array, size_t distance);

void print_npcs(const set_t &array);

void simulate_fight(set_t &array, size_t max_fight_distance);

void generate_npcs(set_t &array, size_t count, size_t field_size);

#endif