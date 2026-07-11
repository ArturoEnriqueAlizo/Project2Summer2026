cmake_minimum_required(VERSION 3.16)
project(Project2Summer2026Final)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Project2Summer2026Final main.cpp
        DataLoader.cpp
        DataLoader.h
        Recommendation.cpp
        Recommendation.h
        Performance.cpp
        Performance.h
        Watchlist.cpp
        Watchlist.h
        Movie.h)

enable_testing()

add_executable(ProjectTests ProjectTests.cpp
        Recommendation.cpp
        Recommendation.h
        Performance.cpp
        Performance.h
        Watchlist.cpp
        Watchlist.h
        Movie.h)

add_test(NAME ProjectTests COMMAND ProjectTests)

# keep the gui optional so the console project remains portable
if(WIN32)
    add_executable(MementoGUI WIN32 GuiMain.cpp
            DataLoader.cpp
            DataLoader.h
            Recommendation.cpp
            Recommendation.h
            Performance.cpp
            Performance.h
            Watchlist.cpp
            Watchlist.h
            Movie.h)
endif()
