cmake_minimum_required(VERSION 3.0)
project(CG_P3 C)

set(CMAKE_C_STANDARD 11)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/shaders)
configure_file(${CMAKE_SOURCE_DIR}/shaders/vertex.glsl ${CMAKE_BINARY_DIR}/shaders/vertex.glsl COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/shaders/fragment.glsl ${CMAKE_BINARY_DIR}/shaders/fragment.glsl COPYONLY)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/textures)
configure_file(${CMAKE_SOURCE_DIR}/textures/hedgehog2.png ${CMAKE_BINARY_DIR}/textures/hedgehog2.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/textures/mushroom.png ${CMAKE_BINARY_DIR}/textures/mushroom.png COPYONLY)

add_executable(CG_P3 main.c)

if(UNIX AND NOT APPLE)
    message(STATUS "Linux")
    target_link_libraries(CG_P3 m glfw GLEW GL SOIL)
else()
    message(STATUS "Windows")
    target_link_libraries(CG_P3 m SOIL glu32 opengl32 glfw3 glew32)
endif()