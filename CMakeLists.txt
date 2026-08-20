cmake_minimum_required(VERSION 3.20)
project(xvec CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# executable
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME})

set(FLAGS -Wall -Wextra -Wpedantic -Werror)

target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
