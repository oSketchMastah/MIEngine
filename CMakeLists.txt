
cmake_minimum_required(VERSION 3.5)

project(MITest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS NO)

SET(SOURCE_EXE main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fconcepts")

include_directories(include/EngineIndependent)
include_directories("include/EngineIndependent/Free Helpers")
include_directories(include/EngineIndependent/Templates)
include_directories(include/EngineTesting)


add_subdirectory(src)

add_executable(${PROJECT_NAME} ${SOURCE_EXE})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

