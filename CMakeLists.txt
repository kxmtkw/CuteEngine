cmake_minimum_required(VERSION 3.12)
project(Cute C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Global compilation flags
add_compile_options(-Wall -Wextra -g)

# --- Cute Instructions ---

add_library(CuteInstr STATIC
    Instr/image.c
)

target_include_directories(CuteInstr
    PUBLIC
        Instr/include
)

# --- Cute Engine ---

add_library(CuteEngine STATIC
    Engine/engine/engine.c
    Engine/engine/exec.c
    Engine/engine/context.c
    Engine/objects/manager.c
    Engine/container/container.c
    Engine/utils/utils.c
)

target_include_directories(CuteEngine
    PUBLIC
        Engine/include
    PRIVATE
        Engine
)

# CMake handles transitive dependencies automatically via PUBLIC linking
target_link_libraries(CuteEngine PUBLIC CuteInstr)

# --- cute binary ---

add_executable(cute main/engine.c)
# Linking CuteEngine automatically pulls in CuteInstr
target_link_libraries(cute PRIVATE CuteEngine)

# --- Cute Assembler ---

add_library(CuteAssembler STATIC
    Assembler/tokenizer/tokenizer.cpp
    Assembler/tokenizer/stream.cpp
    Assembler/assembler/assembler.cpp
)

target_include_directories(CuteAssembler
    PUBLIC
        Assembler/include
    PRIVATE
        Assembler
)

target_link_libraries(CuteAssembler PUBLIC CuteInstr)

# --- cuteasm binary ---

add_executable(cuteasm main/assembler.cpp)
target_link_libraries(cuteasm PRIVATE CuteAssembler)