#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <cstdio>

#include "../include/npc.h"
#include "../include/bandit.h"
#include "../include/ork.h"
#include "../include/werewolf.h"
#include "../include/editor.h"

// -------------------- NPC base tests --------------------

TEST(NPCTest, DistanceCheck) {
    auto a = std::make_shared<Bandit>(0, 0);
    auto b = std::make_shared<Ork>(3, 4); // расстояние 5

    EXPECT_TRUE(a->is_close(b, 5));
    EXPECT_FALSE(a->is_close(b, 4));
}

// -------------------- Fight logic tests --------------------

static std::shared_ptr<Bandit> makeBandit(int x, int y) {
    return std::make_shared<Bandit>(x, y);
}

static std::shared_ptr<Ork> makeOrk(int x, int y) {
    return std::make_shared<Ork>(x, y);
}

static std::shared_ptr<Werewolf> makeWerewolf(int x, int y) {
    return std::make_shared<Werewolf>(x, y);
}

TEST(FightLogicTest, BanditVsOthers) {
    auto b = makeBandit(0, 0);
    auto o = makeOrk(0, 0);
    auto w = makeWerewolf(0, 0);

    // Разбойник атакует орка — не убивает
    EXPECT_FALSE(o->accept(b));

    // Разбойник атакует оборотня — убивает
    EXPECT_TRUE(w->accept(b));

    // Разбойник против разбойника — никто не погибает
    auto b2 = makeBandit(0, 0);
    EXPECT_FALSE(b2->accept(b));
}

TEST(FightLogicTest, OrkVsOthers) {
    auto b = makeBandit(0, 0);
    auto o = makeOrk(0, 0);
    auto w = makeWerewolf(0, 0);

    // Орк атакует разбойника — убивает
    EXPECT_TRUE(b->accept(o));

    // Орк атакует орка — никто не погибает
    auto o2 = makeOrk(0, 0);
    EXPECT_FALSE(o2->accept(o));

    // Орк атакует оборотня — не убивает
    EXPECT_FALSE(w->accept(o));
}

TEST(FightLogicTest, WerewolfVsOthers) {
    auto b = makeBandit(0, 0);
    auto o = makeOrk(0, 0);
    auto w = makeWerewolf(0, 0);

    // Оборотень атакует разбойника — убивает
    EXPECT_TRUE(b->accept(w));

    // Оборотень атакует орка — не убивает
    EXPECT_FALSE(o->accept(w));

    // Оборотень атакует оборотня — никто не погибает
    auto w2 = makeWerewolf(0, 0);
    EXPECT_FALSE(w2->accept(w));
}

// -------------------- Editor: factory, save/load, fight --------------------

TEST(EditorTest, FactoryByTypeAndStream) {
    // factory по типу и координатам
    auto ork = factory(OrkType, 1, 2);
    ASSERT_NE(ork, nullptr);
    EXPECT_EQ(ork->type, OrkType);
    EXPECT_EQ(ork->x, 1);
    EXPECT_EQ(ork->y, 2);

    // factory из потока: формат "type x y"
    std::stringstream ss;
    ss << static_cast<int>(BanditType) << " " << 10 << " " << 20;
    auto from_stream = factory(ss);
    ASSERT_NE(from_stream, nullptr);
    EXPECT_EQ(from_stream->type, BanditType);
    EXPECT_EQ(from_stream->x, 10);
    EXPECT_EQ(from_stream->y, 20);
}

TEST(EditorTest, SaveAndLoadRoundTrip) {
    set_t s;
    s.insert(std::make_shared<Bandit>(1, 2));
    s.insert(std::make_shared<Ork>(3, 4));
    s.insert(std::make_shared<Werewolf>(5, 6));

    const std::string filename = "test_npc_io.txt";
    save(s, filename);

    set_t loaded = load(filename);
    EXPECT_EQ(s.size(), loaded.size());

    // Проверяем, что типы корректно восстановлены
    int bandits = 0, orks = 0, werewolves = 0;
    for (const auto &npc : loaded) {
        if (npc->type == BanditType) ++bandits;
        if (npc->type == OrkType) ++orks;
        if (npc->type == WerewolfType) ++werewolves;
    }
    EXPECT_EQ(bandits, 1);
    EXPECT_EQ(orks, 1);
    EXPECT_EQ(werewolves, 1);

    std::remove(filename.c_str());
}

TEST(EditorTest, FightFunctionReturnsKilledNpcs) {
    set_t s;
    auto bandit = makeBandit(0, 0);
    auto ork = makeOrk(0, 0);
    auto werewolf = makeWerewolf(100, 100); // далеко

    s.insert(bandit);
    s.insert(ork);
    s.insert(werewolf);

    // При маленькой дистанции только орк может убить разбойника
    auto dead = fight(s, 5);
    ASSERT_EQ(dead.size(), 1u);
    auto killed = *dead.begin();
    EXPECT_EQ(killed->type, BanditType);
}

TEST(EditorTest, SimulateFightReducesSet) {
    set_t s;
    s.insert(makeBandit(0, 0));
    s.insert(makeOrk(0, 0));
    s.insert(makeWerewolf(0, 0));

    size_t initial_size = s.size();
    simulate_fight(s, 50);

    // После серии боёв количество NPC не должно увеличиться
    EXPECT_LE(s.size(), initial_size);
}

TEST(EditorTest, GenerateNPCsProducesNonEmptySet) {
    set_t s;
    generate_npcs(s, 10, 500);  // 10 NPC на поле 500x500
    EXPECT_EQ(s.size(), 10u);

    // Проверяем, что все координаты в пределах поля и типы валидны
    for (const auto& npc : s) {
        EXPECT_GE(npc->x, 0);
        EXPECT_LE(npc->x, 500);
        EXPECT_GE(npc->y, 0);
        EXPECT_LE(npc->y, 500);
        EXPECT_GE(npc->type, BanditType);
        EXPECT_LE(npc->type, WerewolfType);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


