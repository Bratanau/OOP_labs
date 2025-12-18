#include "ork.h"
#include "bandit.h"
#include "werewolf.h"

Ork::Ork(int x, int y) : NPC(OrkType, x, y) {}
Ork::Ork(std::istream &is) : NPC(OrkType, is) {}

void Ork::print()
{
    std::cout << *this;
}

bool Ork::accept(std::shared_ptr<NPC> attacker) {
    FightVisitor visitor(attacker);
    return visitor.visit(std::make_shared<Ork>(*this));
}

void Ork::save(std::ostream &os)
{
    os << OrkType << std::endl;
    NPC::save(os);
}

bool Ork::fight(std::shared_ptr<Bandit> other)
{
    // Орк убивает разбойника
    fight_notify(other, true);
    return true;
}

bool Ork::fight(std::shared_ptr<Ork> other)
{
    // Орк НЕ убивает другого орка
    return false;
}

bool Ork::fight(std::shared_ptr<Werewolf> other)
{
    // Орк НЕ убивает оборотня
    return false;
}

std::ostream &operator<<(std::ostream &os, Ork &ork)
{
    os << "ork: " << *static_cast<NPC *>(&ork) << std::endl;
    return os;
}