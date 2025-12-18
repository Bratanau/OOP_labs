#include "../include/bandit.h"
#include "../include/ork.h"
#include "../include/werewolf.h"
#include "../include/npc.h"

Ork::Ork(std::string& name, int x, int y) : NPC(OrkType, name, x, y) {}
Ork::Ork(std::istream &is) : NPC(OrkType, is) {}
void Ork::print() {
    my_print() << *this;
}

bool Ork::accept(std::shared_ptr<NPC> attacker) {
    FightVisitor visitor(attacker);
    return visitor.visit(std::static_pointer_cast<Ork>(shared_from_this()));
}

void Ork::save(std::ostream &os)
{
    os << "Ork ";
    NPC::save(os);
}

bool Ork::fight(std::shared_ptr<Werewolf> other)
{
    fight_notify(other, false);
    return false;
}

bool Ork::fight(std::shared_ptr<Ork> other)
{
    fight_notify(other, false);
    return false;
}

bool Ork::fight(std::shared_ptr<Bandit> other)
{
    fight_notify(other, true);
    return true;
}

std::ostream &operator<<(std::ostream &os, Ork &dr)
{
    os << "Ork: " << *static_cast<NPC *>(&dr) << std::endl;
    return os;
}