cmake_minimum_required(VERSION 3.10)

project(lab_06)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(lab_06 
    main.cpp 
    src/npc.cpp
    src/bandit.cpp
    src/ork.cpp
    src/werewolf.cpp
)