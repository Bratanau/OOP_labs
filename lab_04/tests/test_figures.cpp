#include <gtest/gtest.h>
#include "../include/figures.h"
#include <sstream>
#include <cmath>
#include <vector>
#include <memory>
#include <array>

using namespace std;

using TriangleD = Triangle<double>;
using HexagonD  = Hexagon<double>;
using OctagonD  = Octagon<double>;
using FigureD   = Figure<double>;

// Вспомогательные функции для создания тестовых фигур
unique_ptr<TriangleD> createTestTriangle() {
    array<Point<double>, 3> pts = {
        Point<double>(0.0, 0.0),
        Point<double>(3.0, 0.0),
        Point<double>(0.0, 4.0)
    };
    return make_unique<TriangleD>(pts);
}

unique_ptr<HexagonD> createTestHexagon() {
    Point<double> center(0.0, 0.0);
    double r = 1.0;
    return make_unique<HexagonD>(center, r);
}

unique_ptr<OctagonD> createTestOctagon() {
    Point<double> center(1.0, 2.0);
    double r = 2.0;
    return make_unique<OctagonD>(center, r);
}

// --- Базовые тесты площадей и центров ---

TEST(FigureTest, TriangleAreaAndCenter) {
    auto tri = createTestTriangle();
    EXPECT_NEAR(static_cast<double>(*tri), 6.0, 1e-6);
    auto center = tri->geometricCenter();
    EXPECT_NEAR(center.x, 1.0, 1e-6);
    EXPECT_NEAR(center.y, 4.0 / 3.0, 1e-6);
}

TEST(FigureTest, HexagonAreaAndCenter) {
    auto hex = createTestHexagon();
    // Формула в коде: (3 * sqrt(3) / 2) * r^2, r = 1
    double expected = (3.0 * std::sqrt(3.0) / 2.0);
    EXPECT_NEAR(static_cast<double>(*hex), expected, 1e-6);
    auto center = hex->geometricCenter();
    EXPECT_NEAR(center.x, 0.0, 1e-6);
    EXPECT_NEAR(center.y, 0.0, 1e-6);
}

TEST(FigureTest, OctagonAreaAndCenter) {
    auto oct = createTestOctagon();
    // В коде своя формула, здесь просто проверяем на положительность и центр
    EXPECT_GT(static_cast<double>(*oct), 0.0);
    auto center = oct->geometricCenter();
    EXPECT_NEAR(center.x, 1.0, 1e-6);
    EXPECT_NEAR(center.y, 2.0, 1e-6);
}

// --- Ввод/вывод ---

TEST(FigureTest, TriangleInputOutput) {
    TriangleD tri;
    istringstream iss("0 0 3 0 0 4");
    iss >> static_cast<FigureD&>(tri);

    EXPECT_NEAR(static_cast<double>(tri), 6.0, 1e-6);

    ostringstream oss;
    oss << static_cast<const FigureD&>(tri);
    string output = oss.str();
    EXPECT_NE(output.find("Triangle"), string::npos);
    EXPECT_NE(output.find("vertices"), string::npos);
}

// --- Копирование и перемещение ---

TEST(CopyMoveTest, TriangleCopyAndMove) {
    auto tri1 = createTestTriangle();
    double original_area = static_cast<double>(*tri1);

    // Copy constructor
    TriangleD tri2(*tri1);
    EXPECT_NEAR(static_cast<double>(tri2), original_area, 1e-6);
    EXPECT_TRUE(*tri1 == tri2);

    // Copy assignment через базовый класс
    TriangleD tri3;
    tri3 = static_cast<const FigureD&>(*tri1);
    EXPECT_NEAR(static_cast<double>(tri3), original_area, 1e-6);
    EXPECT_TRUE(*tri1 == tri3);

    // Move constructor
    TriangleD tri4(std::move(*tri1));
    EXPECT_NEAR(static_cast<double>(tri4), original_area, 1e-6);

    // Move assignment
    TriangleD tri5;
    tri5 = std::move(tri4);
    EXPECT_NEAR(static_cast<double>(tri5), original_area, 1e-6);
}

// --- Сравнение фигур ---

TEST(ComparisonTest, TriangleEqualityAndInequality) {
    array<Point<double>, 3> v1 = {
        Point<double>(0, 0),
        Point<double>(3, 0),
        Point<double>(0, 4)
    };
    array<Point<double>, 3> v2 = v1;
    array<Point<double>, 3> v3 = {
        Point<double>(0, 0),
        Point<double>(4, 0),
        Point<double>(0, 3)
    };

    TriangleD t1(v1);
    TriangleD t2(v2);
    TriangleD t3(v3);

    EXPECT_TRUE(t1 == t2);
    EXPECT_FALSE(t1 == t3);
}

TEST(ComparisonTest, DifferentTypesNotEqual) {
    TriangleD tri;
    HexagonD hex;
    OctagonD oct;

    const FigureD& f_tri = tri;
    const FigureD& f_hex = hex;
    const FigureD& f_oct = oct;

    EXPECT_FALSE(tri.operator==(f_hex));
    EXPECT_FALSE(tri.operator==(f_oct));
    EXPECT_FALSE(hex.operator==(f_oct));
}

// --- Полиморфизм и clone ---

TEST(PolymorphismTest, VectorOfFiguresAndClone) {
    vector<shared_ptr<FigureD>> figures;
    figures.push_back(createTestTriangle());
    figures.push_back(createTestHexagon());
    figures.push_back(createTestOctagon());

    ASSERT_EQ(figures.size(), 3u);

    double total_area = 0.0;
    for (const auto& fig : figures) {
        total_area += static_cast<double>(*fig);
        auto center = fig->geometricCenter();
        EXPECT_FALSE(std::isnan(center.x));
        EXPECT_FALSE(std::isnan(center.y));

        ostringstream oss;
        oss << *fig;
        EXPECT_FALSE(oss.str().empty());
    }
    EXPECT_GT(total_area, 0.0);

    // Проверка clone
    auto tri_clone = figures[0]->clone();
    EXPECT_TRUE(*tri_clone == *figures[0]);
    EXPECT_NEAR(static_cast<double>(*tri_clone),
                static_cast<double>(*figures[0]), 1e-6);
}

// --- Тесты для Array<Figure<double>> ---

TEST(ArrayTest, AddRemoveAndTotalArea) {
    Array<FigureD> arr;

    auto tri = createTestTriangle();
    auto hex = createTestHexagon();

    double area_tri = static_cast<double>(*tri);
    double area_hex = static_cast<double>(*hex);

    arr.add(std::move(tri));
    arr.add(std::move(hex));

    EXPECT_EQ(arr.size(), 2u);
    EXPECT_NEAR(arr.totalArea(), area_tri + area_hex, 1e-6);

    arr.remove(0);
    EXPECT_EQ(arr.size(), 1u);
    EXPECT_NEAR(arr.totalArea(), area_hex, 1e-6);
}

TEST(ArrayTest, PrintAllFiguresInfo) {
    Array<FigureD> arr;
    arr.add(createTestTriangle());
    arr.add(createTestHexagon());

    // Перенаправляем cout
    std::streambuf* old_buf = std::cout.rdbuf();
    std::ostringstream oss;
    std::cout.rdbuf(oss.rdbuf());

    arr.printAll();

    std::cout.rdbuf(old_buf); // восстановить

    std::string out = oss.str();
    EXPECT_NE(out.find("Figure 1"), std::string::npos);
    EXPECT_NE(out.find("Figure 2"), std::string::npos);
    EXPECT_NE(out.find("Geometric center"), std::string::npos);
    EXPECT_NE(out.find("Area"), std::string::npos);
}

// --- Крайние случаи ---

TEST(EdgeCaseTest, DefaultConstructorsNonNegativeArea) {
    TriangleD tri;
    HexagonD hex;
    OctagonD oct;

    EXPECT_GE(static_cast<double>(tri), 0.0);
    EXPECT_GE(static_cast<double>(hex), 0.0);
    EXPECT_GE(static_cast<double>(oct), 0.0);

    auto c1 = tri.geometricCenter();
    auto c2 = hex.geometricCenter();
    auto c3 = oct.geometricCenter();

    EXPECT_FALSE(std::isnan(c1.x));
    EXPECT_FALSE(std::isnan(c1.y));
    EXPECT_FALSE(std::isnan(c2.x));
    EXPECT_FALSE(std::isnan(c2.y));
    EXPECT_FALSE(std::isnan(c3.x));
    EXPECT_FALSE(std::isnan(c3.y));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}