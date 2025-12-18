#include "bandit.h"
#include "ork.h"
#include "werewolf.h"

Bandit::Bandit(int x, int y) : NPC(BanditType, x, y) {}
Bandit::Bandit(std::istream &is) : NPC(BanditType, is) {}

void Bandit::print()
{
    std::cout << *this;
}

bool Bandit::accept(std::shared_ptr<NPC> attacker) {
    FightVisitor visitor(attacker);
    return visitor.visit(std::make_shared<Bandit>(*this));
}

void Bandit::save(std::ostream &os)
{
    os << BanditType << std::endl;
    NPC::save(os);
}

bool Bandit::fight(std::shared_ptr<Bandit> other)
{
    // Разбойник НЕ убивает другого разбойника
    return false;
}

bool Bandit::fight(std::shared_ptr<Ork> other)
{
    // Разбойник НЕ убивает орка
    return false;
}

bool Bandit::fight(std::shared_ptr<Werewolf> other)
{
    // Разбойник убивает оборотня
    fight_notify(other, true);
    return true;
}

std::ostream &operator<<(std::ostream &os, Bandit &bandit)
{
    os << "bandit: " << *static_cast<NPC *>(&bandit) << std::endl;
    return os;
}