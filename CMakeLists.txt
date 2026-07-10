cmake_minimum_required(VERSION 4.0)
project(Project2Summer2026Final)

set(CMAKE_CXX_STANDARD 20)

add_executable(Project2Summer2026Final main.cpp
        DataLoader.cpp
        DataLoader.h
        Recommendation.cpp
        Recommendation.h
        Performance.cpp
        Performance.h
        Movie.h)

enable_testing()

add_executable(ProjectTests ProjectTests.cpp
        Recommendation.cpp
        Recommendation.h
        Performance.cpp
        Performance.h
        Movie.h)

add_test(NAME ProjectTests COMMAND ProjectTests)
