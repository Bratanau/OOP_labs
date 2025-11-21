#pragma once
#include "npc.h"

struct Ork : public NPC
{
    Ork(std::string& name, int x, int y);
    Ork(std::istream &is);

    void print() override;
    void save(std::ostream &os) override;

    bool accept(std::shared_ptr<NPC> attacker) override;
    bool fight(std::shared_ptr<Bandit> other) override;
    bool fight(std::shared_ptr<Ork> other) override;
    bool fight(std::shared_ptr<Werewolf> other) override;
    friend std::ostream &operator<<(std::ostream &os, Ork &knight);

    static constexpr NPCParams PARAMS{50, 30};
    const NPCParams& params() const override { return PARAMS; }
};