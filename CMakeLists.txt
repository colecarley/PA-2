cmake_minimum_required(VERSION 3.31) # Minimum required CMake version
set(CMAKE_CXX_STANDARD 14)
project(graphics) # Project name

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(graphics Code/main.cpp 
                        Code/camera.cpp
                        Code/camera.h
                        Code/engine.cpp
                        Code/engine.h
                        Code/graphics.cpp
                        Code/graphics.h
                        Code/graphics_headers.h
                        Code/mesh.cpp
                        Code/mesh.h
                        Code/object.cpp
                        Code/object.h
                        Code/shader.cpp
                        Code/shader.h
                        Code/sphere.cpp
                        Code/sphere.h
												Code/texture.cpp
												Code/texture.h
                        Code/window.cpp
                        Code/window.h
												Code/skybox.h
												Code/skybox.cpp
												Code/material.h
												Code/mode.h
												Code/ring.h
												Code/ring.cpp
												Code/asteroid_belt.cpp
												Code/asteroid_belt.h
												Code/solar_constants.h
												Code/shader_variable_locations.h
												Code/line.cpp
												Code/line.h
                        )

target_compile_options(graphics PUBLIC -Wall -Wpedantic) 

target_include_directories(graphics PUBLIC /opt/homebrew/opt/)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(soil2 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

target_link_libraries(graphics 
	glfw
	GLEW::glew
	OpenGL::GL
	assimp::assimp
	soil2::soil2
	"-framework CoreFoundation"
)

