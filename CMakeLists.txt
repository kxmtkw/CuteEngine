cmake_minimum_required(VERSION 3.15)
project(CuteVM C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Global compiler configuration
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

# Set output directory to .build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.build")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.build")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/.build")

# Common compiler flags
set(COMMON_FLAGS -Wall -Wextra -g)

# ==============================================================================
# Cute Engine (Static Library)
# ==============================================================================
add_library(CuteEngine STATIC
    Engine/engine/engine.c
    Engine/engine/exec.c
    Engine/engine/context.c
    Engine/object/object.c
    Engine/object/container.c
    Engine/image/image.c
    Engine/utils/utils.c
)

target_include_directories(CuteEngine
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Engine/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Engine
)

target_compile_options(CuteEngine PRIVATE ${COMMON_FLAGS})

# ==============================================================================
# Engine Entry Point (Executable: cute)
# ==============================================================================
add_executable(cute main/main.c)

target_compile_options(cute PRIVATE ${COMMON_FLAGS})
target_link_libraries(cute PRIVATE CuteEngine)

# ==============================================================================
# Assembler (Executable: cuteasm)
# ==============================================================================
add_executable(cuteasm
    Assembler/main.cpp
    Assembler/tokenizer/tokenizer.cpp
    Assembler/tokenizer/stream.cpp
    Assembler/parser/parser.cpp
    Assembler/codegen/codegen.cpp
)

target_include_directories(cuteasm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Assembler)
target_compile_options(cuteasm PRIVATE ${COMMON_FLAGS})
target_link_libraries(cuteasm PRIVATE CuteEngine)