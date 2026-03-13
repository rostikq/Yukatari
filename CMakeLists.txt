cmake_minimum_required(VERSION 4.1)
project(Yukatari)

set(CMAKE_CXX_STANDARD 20)


include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)


add_executable(Yukatari main.cpp
        src/core/Application.cpp
        src/core/Application.h
        src/Debug.h
        src/core/Timer.h
        src/core/State.h)

target_link_libraries(Yukatari sfml-graphics sfml-window sfml-system sfml-network)