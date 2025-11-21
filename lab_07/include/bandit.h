#pragma once
#include "npc.h"

struct Bandit : public NPC
{
    Bandit(std::string& name, int x, int y);
    Bandit(std::istream &is);

    void print() override;
    void save(std::ostream &os) override;

    bool accept(std::shared_ptr<NPC> attacker) override;
    bool fight(std::shared_ptr<Ork> other) override;
    bool fight(std::shared_ptr<Werewolf> other) override;
    bool fight(std::shared_ptr<Bandit> other) override;
    friend std::ostream &operator<<(std::ostream &os, Bandit &knight);

    static constexpr NPCParams PARAMS{30, 10};
    const NPCParams& params() const override { return PARAMS; }
};