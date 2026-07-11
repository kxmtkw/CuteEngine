
file(GLOB TOKENIZER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/tokenizer/*.cpp")
file(GLOB PARSER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/parser/*.cpp")
file(GLOB RESOLVER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/resolver/*.cpp")
file(GLOB PARSER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/parser/*.cpp")

add_executable(
	cuteasm
    main.cpp
	${TOKENIZER_SRC}
	${PARSER_SRC}
	${RESOLVER_SRC}
)

target_link_libraries(cuteasm PRIVATE CuteEngine)
target_compile_options(
    cuteasm
    PRIVATE 
    -Wall -Wextra -g
)

target_include_directories(
    cuteasm
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(
    cuteasm
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)