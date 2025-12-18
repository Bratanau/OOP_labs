#pragma once
#include "npc.h"

struct Bandit : public NPC
{
    Bandit(int x, int y);
    Bandit(std::istream &is);

    void print() override;
    void save(std::ostream &os) override;

    bool accept(std::shared_ptr<NPC> attacker) override;
    bool fight(std::shared_ptr<Bandit> other) override;
    bool fight(std::shared_ptr<Ork> other) override;
    bool fight(std::shared_ptr<Werewolf> other) override;
    friend std::ostream &operator<<(std::ostream &os, Bandit &bandit);
};