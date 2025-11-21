#include "../include/Bull.h"
#include "../include/Toad.h"
#include "../include/Dragon.h"
#include "../include/npc.h"

Toad::Toad(std::string& name, int x, int y) : NPC(ToadType, name, x, y) {}
Toad::Toad(std::istream &is) : NPC(ToadType, is) {}
void Toad::print() {
    my_print() << *this;
}

bool Toad::accept(std::shared_ptr<NPC> attacker) {
    FightVisitor visitor(attacker);
    return visitor.visit(std::static_pointer_cast<Toad>(shared_from_this()));
}

void Toad::save(std::ostream &os)
{
    os << "toad ";
    NPC::save(os);
}

bool Toad::fight(std::shared_ptr<Toad> other)
{
    fight_notify(other, false);
    return false;
}

bool Toad::fight(std::shared_ptr<Dragon> other)
{
    fight_notify(other, false);
    return false;
}

bool Toad::fight(std::shared_ptr<Bull> other)
{
    fight_notify(other, false);
    return false;
}

std::ostream &operator<<(std::ostream &os, Toad &toad)
{
    os << "toad: " << *static_cast<NPC *>(&toad) << std::endl;
    return os;
}