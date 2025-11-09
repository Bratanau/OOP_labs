cmake_minimum_required(VERSION 3.10)
project(lab_03 LANGUAGES CXX)

add_executable(lab_03_run
    main.cpp
    src/figure.cpp
    src/pentagon.cpp
    src/rhombus.cpp
    src/trapezoid.cpp
)

add_executable(lab_03_tests
    tests/main_tests.cpp
    src/figure.cpp
    src/pentagon.cpp
    src/rhombus.cpp
    src/trapezoid.cpp
)

target_link_libraries(lab_03_tests PRIVATE gtest gtest_main)

target_include_directories(lab_03_run PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(lab_03_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)