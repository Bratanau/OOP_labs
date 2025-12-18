#include "../include/bandit.h"
#include "../include/ork.h"
#include "../include/werewolf.h"
#include "../include/npc.h"

Werewolf::Werewolf(std::string& name, int x, int y) : NPC(WerewolfType, name, x, y) {}
Werewolf::Werewolf(std::istream &is) : NPC(WerewolfType, is) {}
void Werewolf::print() {
    my_print() << *this;
}

bool Werewolf::accept(std::shared_ptr<NPC> attacker) {
    FightVisitor visitor(attacker);
    return visitor.visit(std::static_pointer_cast<Werewolf>(shared_from_this()));
}

void Werewolf::save(std::ostream &os)
{
    os << "Werewolf ";
    NPC::save(os);
}

bool Werewolf::fight(std::shared_ptr<Werewolf> other)
{
    fight_notify(other, false);
    return false;
}

bool Werewolf::fight(std::shared_ptr<Ork> other)
{
    fight_notify(other, false);
    return false;
}

bool Werewolf::fight(std::shared_ptr<Bandit> other)
{
    fight_notify(other, false);
    return true;
}

std::ostream &operator<<(std::ostream &os, Werewolf &Werewolf)
{
    os << "Werewolf: " << *static_cast<NPC *>(&Werewolf) << std::endl;
    return os;
}