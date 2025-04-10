cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(MaterialTest)
# set(CMAKE_CXX_COMPILER  clang++)
# set(CMAKE_CXX_COMPILER_TARGET x86_64-w64-windows-gnu)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/GLFW)

set(SOURCES
	main.cpp
	object.cpp
        object.h
	shader.cpp
        shader.h
	skeletalanimation.cpp
        skeletalanimation.h
        lodePNG.cpp
        glad.c
	physicssystem.cpp
        physicssystem.h
	resources.cpp
        resources.h
	collision.h
	collision.cpp 
        mymath.cpp
        mymath.h)

add_executable(MaterialTest ${SOURCES})

target_link_libraries(MaterialTest glfw3)
