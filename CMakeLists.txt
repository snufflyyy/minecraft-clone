cmake_minimum_required(VERSION 3.28)
project(BradensMinecraftClone C)

set(CMAKE_C_STANDARD 99)

cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(external/glfw)

include_directories(
	include
	external/glfw/include
	external/glad/include
	external/stb/include
)

add_executable (
	${PROJECT_NAME}
	src/main.c
	src/window.c
	src/shader.c
	src/texture.c
	src/camera.c
	src/block.c
	src/chunk.c
	src/world.c
	src/game.c

	src/math/matrix4f.c
	src/math/vector3f.c

	external/glad/src/glad.c
)

target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)