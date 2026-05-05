cmake_minimum_required(VERSION 3.20)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled main.cpp
        Bug.cpp
        Bug.h
        Crawler.cpp
        Crawler.h
        Hopper.cpp
        Hopper.h
        KnightBug.cpp
        KnightBug.h
        Board.cpp
        Board.h)
