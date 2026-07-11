

add_executable(
	cuteasm
    main.cpp
	
	tokenizer/tokenizer.cpp
	tokenizer/stream.cpp

	parser/parser.cpp

	codegen/codegen.cpp
)

target_link_libraries(cuteasm PRIVATE CuteEngine)

target_compile_options(
    cuteasm
    PRIVATE 
    -Wall -Wextra -g
)

target_include_directories(
    cuteasm
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(
    cuteasm
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)