cmake_minimum_required(VERSION 3.5.0)

project(tests VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")

add_executable(${PROJECT_NAME} tests.cc)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)

# -Wall -Wextra -Wpedantic
set(CMAKE_CXX_FLAGS_INIT "-Werror -c -g -rdynamic -fpermissive -Wfatal-errors -fcompare-debug-second" )

add_definitions(-std=c++17 -O3) # -m64)

include_directories("../lib" "../libs")