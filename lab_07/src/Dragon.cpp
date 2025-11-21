#include "../include/Bull.h"
#include "../include/Toad.h"
#include "../include/Dragon.h"
#include "../include/npc.h"

Dragon::Dragon(std::string& name, int x, int y) : NPC(DragonType, name, x, y) {}
Dragon::Dragon(std::istream &is) : NPC(DragonType, is) {}
void Dragon::print() {
    my_print() << *this;
}

bool Dragon::accept(std::shared_ptr<NPC> attacker) {
    FightVisitor visitor(attacker);
    return visitor.visit(std::static_pointer_cast<Dragon>(shared_from_this()));
}

void Dragon::save(std::ostream &os)
{
    os << "dragon ";
    NPC::save(os);
}

bool Dragon::fight(std::shared_ptr<Toad> other)
{
    fight_notify(other, false);
    return false;
}

bool Dragon::fight(std::shared_ptr<Dragon> other)
{
    fight_notify(other, false);
    return false;
}

bool Dragon::fight(std::shared_ptr<Bull> other)
{
    fight_notify(other, true);
    return true;
}

std::ostream &operator<<(std::ostream &os, Dragon &dr)
{
    os << "dragon: " << *static_cast<NPC *>(&dr) << std::endl;
    return os;
}