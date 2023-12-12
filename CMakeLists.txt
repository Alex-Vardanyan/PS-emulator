cmake_minimum_required(VERSION 3.26)
project(SGK)

set(CMAKE_CXX_STANDARD 17)

add_executable(SGK main.cpp
        Range.cpp
        Range.h
        Interconnect.cpp
        Interconnect.h
        CPU.cpp
        CPU.h
        BIOS.cpp
        BIOS.h
        Instruction.cpp
        Instruction.h
)
