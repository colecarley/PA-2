cmake_minimum_required(VERSION 3.31) # Minimum required CMake version
set(CMAKE_CXX_STANDARD 14)
project(graphics) # Project name

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(graphics Code/main.cpp 
                        Code/camera.cpp
                        Code/engine.cpp
                        Code/graphics.cpp
                        Code/object.cpp
                        Code/shader.cpp
                        Code/shader.cpp
                        Code/window.cpp
                        Code/sphere.cpp
                        Code/camera.h
                        Code/mesh.cpp
                        Code/engine.h
                        Code/graphics_headers.h
                        Code/graphics.h
                        Code/object.h
                        Code/shader.h
                        Code/sphere.h
                        Code/window.h
                        Code/mesh.h
                        )

target_compile_options(graphics PUBLIC -Wall -Wpedantic)

target_include_directories(graphics 
PRIVATE /opt/homebrew/opt/glfw/include 
PRIVATE /opt/homebrew/opt/glew/include
PRIVATE /opt/homebrew/opt/glm/include)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(graphics glfw)
target_link_libraries(graphics GLEW::glew)
target_link_libraries(graphics OpenGL::GL)
