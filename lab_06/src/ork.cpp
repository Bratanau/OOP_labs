#include "ork.h"
#include "bandit.h"
#include "werewolf.h"

Ork::Ork(int x, int y) : NPC(OrkType, x, y) {}
Ork::Ork(std::istream &is) : NPC(OrkType, is) {}

void Ork::print()
{
    std::cout << *this;
}

void Ork::save(std::ostream &os)
{
    os << OrkType << std::endl;
    NPC::save(os);
}

bool Ork::is_ork() const
{
    return true;
}

bool Ork::fight(std::shared_ptr<Bandit> other)
{
    fight_notify(other, true);
    return true;
}

bool Ork::fight(std::shared_ptr<Ork> other)
{
    fight_notify(other, true);
    return true;
}

bool Ork::fight(std::shared_ptr<Werewolf> other)
{
    fight_notify(other, true);
    return true;
}

std::ostream &operator<<(std::ostream &os, Ork &ork)
{
    os << "ork: " << *static_cast<NPC *>(&ork) << std::endl;
    return os;
}