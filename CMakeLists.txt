cmake_minimum_required(VERSION 3.31) # Minimum required CMake version
set(CMAKE_CXX_STANDARD 17)
project(graphics) # Project name

add_executable(graphics Code/main.cpp 
                        Code/camera.cpp
                        Code/engine.cpp
                        Code/graphics.cpp
                        Code/object.cpp
                        Code/pyramid.cpp
                        Code/shader.cpp
                        Code/shader.cpp
                        Code/window.cpp
                        Code/cube.cpp
                        Code/camera.h
                        Code/pyramid.h
                        Code/cube.h
                        Code/engine.h
                        Code/graphics_headers.h
                        Code/graphics.h
                        Code/object.h
                        Code/shader.h
                        Code/window.h
                        )

target_include_directories(graphics 
PRIVATE /opt/homebrew/opt/glfw/include 
PRIVATE /opt/homebrew/opt/glew/include
PRIVATE /opt/homebrew/opt/glm/include)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(graphics GLEW::glew)
target_link_libraries(graphics OpenGL::GL)
target_link_libraries(graphics glfw)