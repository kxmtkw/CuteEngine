
file(GLOB ENGINE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/engine/*.c")
file(GLOB IMAGE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/image/*.c")
file(GLOB OBJECT_SRC "${CMAKE_CURRENT_SOURCE_DIR}/object/*.c")
file(GLOB CONTAINER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/container/*.c")
file(GLOB MODULES_SRC "${CMAKE_CURRENT_SOURCE_DIR}/modules/*.c")
file(GLOB UTILS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.c")


add_library(
	CuteEngine
	STATIC

    ${ENGINE_SRC}
	${IMAGE_SRC}
	${OBJECT_SRC}
	${CONTAINER_SRC}
	${MODULES_SRC}
	${UTILS_SRC}
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