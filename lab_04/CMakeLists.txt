cmake_minimum_required(VERSION 3.10)
project(lab_04 LANGUAGES CXX)

# Используем C++20, т.к. в коде применяются concepts
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab_04_run
    src/main.cpp
    src/figures.cpp
)

add_executable(lab_04_tests
    tests/test_figures.cpp
    src/figures.cpp
)

target_link_libraries(lab_04_tests PRIVATE gtest gtest_main)

target_include_directories(lab_04_run PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(lab_04_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
