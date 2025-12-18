#include "werewolf.h"
#include "bandit.h"
#include "ork.h"

Werewolf::Werewolf(int x, int y) : NPC(WerewolfType, x, y) {}
Werewolf::Werewolf(std::istream &is) : NPC(WerewolfType, is) {}

void Werewolf::print()
{
    std::cout << *this;
}

bool Werewolf::accept(std::shared_ptr<NPC> attacker) {
    FightVisitor visitor(attacker);
    return visitor.visit(std::make_shared<Werewolf>(*this));
}

void Werewolf::save(std::ostream &os)
{
    os << WerewolfType << std::endl;
    NPC::save(os);
}

bool Werewolf::fight(std::shared_ptr<Bandit> other)
{
    // Оборотень убивает разбойника
    fight_notify(other, true);
    return true;
}

bool Werewolf::fight(std::shared_ptr<Ork> other)
{
    // Оборотень НЕ убивает орка
    return false;
}

bool Werewolf::fight(std::shared_ptr<Werewolf> other)
{
    // Оборотень НЕ убивает другого оборотня
    return false;
}

std::ostream &operator<<(std::ostream &os, Werewolf &werewolf)
{
    os << "werewolf: " << *static_cast<NPC *>(&werewolf) << std::endl;
    return os;
}