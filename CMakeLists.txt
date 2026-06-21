cmake_minimum_required(VERSION 3.15)

project(threadpool
	VERSION 1.0.0
	DESCRIPTION "A threadpool library"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES threadpool.cpp)

#shared
add_library(threadpool_shared SHARED ${SOURCES})

set_target_properties(threadpool_shared PROPERTIES
	OUTPUT_NAME threadpool
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_include_directories(threadpool_shared PUBLIC .)

# static
add_library(threadpool_static STATIC ${SOURCES})

set_target_properties(threadpool_static PROPERTIES
	OUTPUT_NAME threadpool
)

target_include_directories(threadpool_static PUBLIC .)
