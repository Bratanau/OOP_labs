#pragma once

#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <set>
#include <cmath>
#include <vector>
#include <shared_mutex>
#include "utils.h"

// type for npcs
struct NPC;
struct Ork;
struct Bandit;
struct Werewolf;
using set_t = std::set<std::shared_ptr<NPC>>;

enum NpcType
{
    Unknown = 0,
    BanditType = 1,
    OrkType = 2,
    WerewolfType = 3
};

struct NPCParams {
    int moveDistance;
    int killDistance;
};

struct IFightObserver{
    virtual void on_fight(const std::shared_ptr<NPC> attacker,const std::shared_ptr<NPC> defender,bool win) = 0;
};

class FightVisitor {
    private:
        std::shared_ptr<NPC> attacker;
    public:
        FightVisitor(std::shared_ptr<NPC> atk) : attacker(atk) {}
        virtual ~FightVisitor() = default;
        bool visit(std::shared_ptr<Ork> ork);
        bool visit(std::shared_ptr<Werewolf> werewolf);
        bool visit(std::shared_ptr<Bandit> bandit);
};

struct NPC : public std::enable_shared_from_this<NPC>
{
    mutable std::shared_mutex mtx;
    NpcType type;
    int x{0};
    int y{0};
    std::string name;
    bool alive{true};

    std::vector<std::shared_ptr<IFightObserver>> observers;
    virtual const NPCParams& params() const = 0;
    NPC(NpcType t, std::string& name, int _x, int _y);
    NPC(NpcType t, std::istream &is);

    void subscribe(std::shared_ptr<IFightObserver>observer );
    void fight_notify(const std::shared_ptr<NPC> defender,bool win);
    virtual bool is_close(const std::shared_ptr<NPC> &other, size_t distance) const;
    virtual bool accept(std::shared_ptr<NPC> attacker) = 0;
    virtual bool fight(std::shared_ptr<Bandit> other) = 0;
    virtual bool fight(std::shared_ptr<Ork> other) = 0;
    virtual bool fight(std::shared_ptr<Werewolf> other) = 0;
    virtual void print() = 0;

    virtual void save(std::ostream &os);

    friend std::ostream &operator<<(std::ostream &os, NPC &npc);

    std::pair<int, int> position() const;
    NpcType get_type() const;
    void move(int shift_x, int shift_y, int max_x, int max_y);
    bool is_alive() const;
    void must_die();
    int roll_dice(int sides);
    
    int moveDistance() const;
    int killDistance() const;
};