cmake_minimum_required(VERSION 3.15)
project(CuteEngineProject C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Export compile_commands.json for IntelliSense / clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set global output directory (.build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/.build")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/.build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/.build")

# Common compiler flags
add_compile_options(-Wall -Wextra -g)

# ==============================================================================
# Cute Engine (Static Library)
# ==============================================================================
set(CUTE_ENGINE_SRC "${CMAKE_SOURCE_DIR}/Engine")

add_library(CuteEngine STATIC
    "${CUTE_ENGINE_SRC}/engine/engine.c"
    "${CUTE_ENGINE_SRC}/engine/exec.c"
    "${CUTE_ENGINE_SRC}/engine/context.c"
    "${CUTE_ENGINE_SRC}/object/object.c"
    "${CUTE_ENGINE_SRC}/object/container.c"
    "${CUTE_ENGINE_SRC}/image/image.c"
    "${CUTE_ENGINE_SRC}/utils/utils.c"
)

target_include_directories(CuteEngine
    PUBLIC
        "${CUTE_ENGINE_SRC}/include"
    PRIVATE
        "${CUTE_ENGINE_SRC}"
)

# ==============================================================================
# Cute Assembler (Static Library)
# ==============================================================================
set(CUTE_ASM_SRC "${CMAKE_SOURCE_DIR}/Assembler")

add_library(CuteAssembler STATIC
    "${CUTE_ASM_SRC}/tokenizer/tokenizer.cpp"
    "${CUTE_ASM_SRC}/tokenizer/stream.cpp"
    "${CUTE_ASM_SRC}/parser/parser.cpp"
    "${CUTE_ASM_SRC}/codegen/codegen.cpp"
    "${CUTE_ASM_SRC}/core/core.cpp"
)

target_include_directories(CuteAssembler
    PUBLIC
        "${CUTE_ASM_SRC}/include"
    PRIVATE
        "${CUTE_ASM_SRC}"
)

# Link CuteEngine static lib into CuteAssembler static lib
target_link_libraries(CuteAssembler PUBLIC CuteEngine)

# ==============================================================================
# Entry Points (Executables)
# ==============================================================================

# Engine CLI executable
add_executable(cute
    "${CMAKE_SOURCE_DIR}/main/engine.c"
)
target_link_libraries(cute PRIVATE CuteEngine)

# Assembler CLI executable
add_executable(cuteasm
    "${CMAKE_SOURCE_DIR}/main/assembler.cpp"
)
target_link_libraries(cuteasm PRIVATE CuteAssembler)