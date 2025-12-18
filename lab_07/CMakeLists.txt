cmake_minimum_required(VERSION 3.10)
project(lab_07 LANGUAGES CXX)

# Используем C++20, т.к. в коде применяются concepts
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab_07_run
    main.cpp
    src/bandit.cpp
    src/editor.cpp
    src/npc.cpp
    src/ork.cpp
    src/werewolf.cpp
    src/fightManager.cpp
    src/utils.cpp
)

add_executable(lab_07_tests
    tests/main_test.cpp
    src/bandit.cpp
    src/editor.cpp
    src/npc.cpp
    src/ork.cpp
    src/werewolf.cpp
    src/fightManager.cpp
    src/utils.cpp
)

target_link_libraries(lab_07_tests PRIVATE gtest)

target_include_directories(lab_07_run PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(lab_07_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

enable_testing()
add_test(NAME lab_07_tests COMMAND lab_07_tests)