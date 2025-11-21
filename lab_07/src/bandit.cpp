#include "../include/bandit.h"
#include "../include/ork.h"   
#include "../include/werewolf.h"
#include "../include/npc.h"

Bandit::Bandit(std::string& name, int x, int y) : NPC(BanditType, name, x, y) {}
Bandit::Bandit(std::istream &is) : NPC(BanditType, is) {}
void Bandit::print()
{
    my_print() << *this;
}

bool Bandit::accept(std::shared_ptr<NPC> attacker) {
    FightVisitor visitor(attacker);
    return visitor.visit(std::static_pointer_cast<Bandit>(shared_from_this()));
}

void Bandit::save(std::ostream &os)
{
    os << "Bandit ";
    NPC::save(os);
}

bool Bandit::fight(std::shared_ptr<Ork> other)
{
    fight_notify(other, true);
    return true;
}

bool Bandit::fight(std::shared_ptr<Werewolf> other)
{
    fight_notify(other, false);
    return false;
}

bool Bandit::fight(std::shared_ptr<Bandit> other)
{
    fight_notify(other, false);
    return false;
}

std::ostream &operator<<(std::ostream &os, Bandit &bandit)
{
    os << "Bandit: " << *static_cast<NPC *>(&bandit) << std::endl;
    return os;
}