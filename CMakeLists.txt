cmake_minimum_required(VERSION 3.16)
project(advance_opengl CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Generates compile_commands.json for clangd

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

add_executable(advance_opengl advanceopenglchapter/main.cpp)

target_link_libraries(advance_opengl PRIVATE 
    OpenGL::GL
    glfw
    GLEW::GLEW
    glm::glm
    assimp::assimp
)
