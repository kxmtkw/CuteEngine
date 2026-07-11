

add_library(
	CuteEngine
	STATIC

    engine/engine.c
	engine/context.c
	engine/exec.c
	
	image/image.c

	object/object.c

	container/container.c

	utils/utils.c
)

target_compile_options(
    CuteEngine
    PRIVATE 
    -Wall -Wextra -g
)

target_include_directories(
    CuteEngine
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(
    CuteEngine
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)