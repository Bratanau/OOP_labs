#pragma once
#include "npc.h"

struct Werewolf : public NPC
{
    Werewolf(int x, int y);
    Werewolf(std::istream &is);

    void print() override;
    void save(std::ostream &os) override;

    bool accept(std::shared_ptr<NPC> attacker) override;
    bool fight(std::shared_ptr<Bandit> other) override;
    bool fight(std::shared_ptr<Ork> other) override;
    bool fight(std::shared_ptr<Werewolf> other) override;
    friend std::ostream &operator<<(std::ostream &os, Werewolf &werewolf);
};