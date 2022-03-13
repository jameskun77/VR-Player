cmake_minimum_required(VERSION 2.8)

project(VRPlayer)

configure_file(
	"${PROJECT_SOURCE_DIR}/configuration/root_directory.h.in"
	"${PROJECT_SOURCE_DIR}/includes/root_directory.h"
	)
	
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(SOURCE_DIR src)

set(SOURCE_LIST ${SOURCE_DIR}/glad.cpp
				${SOURCE_DIR}/main.cpp 
				${SOURCE_DIR}/generateSphere.cpp
				${CMAKE_SOURCE_DIR}/includes/generateSphere.h
				${SOURCE_DIR}/sphereProperty.cpp
				${CMAKE_SOURCE_DIR}/includes/sphereProperty.h)
	

message(STATUS "Current directory: " ${CMAKE_SOURCE_DIR})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/includes)

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(VRPlayer ${SOURCE_LIST})

target_link_libraries(VRPlayer glfw3.lib)
