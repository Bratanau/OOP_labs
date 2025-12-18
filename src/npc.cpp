#include "npc.h"

NPC::NPC(NpcType t, int _x, int _y) : type(t), x(_x), y(_y) {}
NPC::NPC(NpcType t, std::istream &is) : type(t) {
    // We store only coordinates for now
    is >> x;
    is >> y;
}

bool FightVisitor::visit(std::shared_ptr<Bandit> bandit) {
    return attacker->fight(bandit);
}

bool FightVisitor::visit(std::shared_ptr<Ork> ork) {
    return attacker->fight(ork);
}

bool FightVisitor::visit(std::shared_ptr<Werewolf> werewolf) {
    return attacker->fight(werewolf);
}

void NPC::subscribe(std::shared_ptr<IFightObserver> observer)
{
   observers.push_back(observer);
}

void NPC::fight_notify(const std::shared_ptr<NPC> defender, bool win)
{
    for (auto &o : observers)
        o->on_fight(shared_from_this(), defender, win);
}

bool NPC::is_close(const std::shared_ptr<NPC> &other, size_t distance) const
{
    return pow((x - other->x), 2) + pow((y - other->y), 2) <= distance * distance;
}

void NPC::save(std::ostream &os)
{
    os << x << std::endl;
    os << y << std::endl;
}

std::ostream &operator<<(std::ostream &os, NPC &npc)
{
    os << "{ x:" << npc.x << ", y:" << npc.y << "} ";
    return os;
}