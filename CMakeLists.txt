
cmake_minimum_required(VERSION 3.5)

project(MITest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS NO)

SET(SOURCE_EXE)
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fconcepts")
endif()
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall") #no support for -fconcepts flag, hoping it is built-in now
endif()
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fconcepts")
endif()
include_directories(include/EngineIndependent)
include_directories(include/EngineIndependent/BasicTypes)
include_directories("include/EngineIndependent/Free Helpers")
include_directories(include/EngineIndependent/Env)
include_directories(include/EngineIndependent/Memory)
include_directories(include/EngineIndependent/Templates)
include_directories(include/EngineIndependent/Parsers)
include_directories(include/EngineTesting)


add_subdirectory(src)

#add_executable(${PROJECT_NAME} ${SOURCE_EXE})
#set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

include(CTest)
enable_testing()
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING))
	SET(TEST_FILES )
	add_subdirectory(tests) #get the tests
	list(LENGTH TEST_FILES NUM_TESTS)
	MATH(EXPR NUM_TESTS "${NUM_TESTS}-1")
	foreach(i RANGE ${NUM_TESTS})
		list(GET TEST_FILES ${i} test)
		add_executable(${test} tests/${test}.cpp ${SOURCE_EXE})
		#set_target_properties(${test} PROPERTIES OUTPUT_NAME ${test})
		add_test(NAME ${test}
			 COMMAND ${test})
	endforeach()
endif()
