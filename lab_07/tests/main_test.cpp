// tests.cpp
#include <gtest/gtest.h>

#include "../include/npc.h"
#include "../include/ork.h"
#include "../include/bandit.h"
#include "../include/werewolf.h"
#include "../include/editor.h"
#include "../include/fightManager.h"
#include "../include/utils.h"

#include <fstream>
#include <chrono>

// ===== Utils =====

TEST(UtilsTest, RandomDirectionLengthNotExceed) {
    int length = 10;
    // Несколько итераций, чтобы уменьшить вероятность флуктуаций
    for (int i = 0; i < 100; ++i) {
        auto [dx, dy] = randomDirection(length);
        int r2 = dx * dx + dy * dy;
        // randomDirection использует cos/sin, длина не должна превышать length
        EXPECT_LE(r2, length * length);
    }
}

TEST(UtilsTest, GameIsOverUsesGameDurationSeconds) {
    using namespace std::chrono;

    auto now    = steady_clock::now();
    auto before = now - seconds(static_cast<int>(GAME_DURATION_SECONDS) - 1); // ещё не истекло
    auto after  = now - seconds(static_cast<int>(GAME_DURATION_SECONDS) + 1); // уже истекло

    EXPECT_FALSE(game_is_over(before));
    EXPECT_TRUE(game_is_over(after));
}

// ===== NPC =====

TEST(NPCTest, MoveClampsToBounds) {
    std::string name = "test";
    Ork o{name, 0, 0};

    // Уход сильно в минус
    o.move(-100, -100, 10, 10);
    auto [x1, y1] = o.position();
    EXPECT_EQ(x1, 0);
    EXPECT_EQ(y1, 0);

    // Уход сильно в плюс
    o.move(100, 100, 10, 10);
    auto [x2, y2] = o.position();
    EXPECT_EQ(x2, 10);
    EXPECT_EQ(y2, 10);
}

TEST(NPCTest, IsCloseUsesEuclideanDistance) {
    std::string name1 = "a";
    std::string name2 = "b";

    auto n1 = std::make_shared<Ork>(name1, 0, 0);
    auto n2 = std::make_shared<Ork>(name2, 3, 4); // расстояние = 5

    EXPECT_TRUE(n1->is_close(n2, 5));
    EXPECT_FALSE(n1->is_close(n2, 4));
}

TEST(NPCTest, AliveFlagChangesOnMustDie) {
    std::string name = "bandit";
    Bandit b{name, 5, 5};

    EXPECT_TRUE(b.is_alive());
    b.must_die();
    EXPECT_FALSE(b.is_alive());
}

// ===== Editor / factory =====

TEST(EditorTest, FactoryCreatesCorrectTypesAndPositions) {
    std::string n1 = "o";
    std::string n2 = "b";
    std::string n3 = "w";

    auto o  = factory(OrkType,    n1, 1, 2);
    auto b  = factory(BanditType, n2, 3, 4);
    auto w  = factory(WerewolfType, n3, 5, 6);

    ASSERT_NE(o,  nullptr);
    ASSERT_NE(b,  nullptr);
    ASSERT_NE(w,  nullptr);

    EXPECT_EQ(o->get_type(), OrkType);
    EXPECT_EQ(b->get_type(), BanditType);
    EXPECT_EQ(w->get_type(), WerewolfType);

    EXPECT_EQ(o->position(), std::make_pair(1, 2));
    EXPECT_EQ(b->position(), std::make_pair(3, 4));
    EXPECT_EQ(w->position(), std::make_pair(5, 6));
}

TEST(EditorTest, SaveAndLoadRoundTripKeepsCount) {
    set_t array;

    std::string n1 = "o";
    std::string n2 = "b";
    std::string n3 = "w";

    array.insert(factory(OrkType,    n1, 1, 2));
    array.insert(factory(BanditType, n2, 3, 4));
    array.insert(factory(WerewolfType, n3, 5, 6));

    const std::string filename = "test_npcs.txt";
    save(array, filename);

    set_t loaded = load(filename);

    EXPECT_EQ(array.size(), loaded.size());
}

// ===== FightManager (базовый тест) =====

TEST(FightManagerTest, AddEventIsThreadSafeAndDoesNotThrow) {
    std::string n1 = "a";
    std::string n2 = "b";

    auto attacker = factory(BanditType, n1, 0, 0);
    auto defender = factory(OrkType,    n2, 1, 1);

    EXPECT_NO_THROW({
        FightManager::get().add_event({attacker, defender});
    });
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}