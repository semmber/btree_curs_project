cmake_minimum_required(VERSION 3.27)
project(btrees)

set(CMAKE_CXX_STANDARD 20)

add_executable(btrees main.cpp
        BTree.cpp
        BTree.h
        RunTest.h
        BPlusTree.cpp
        BPlusTree.h)
