project("LochNesEmulator")
cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 17)

# Find SDL2 library
find_package(SDL2 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})

file(GLOB HARDWARE_SOURCE "${PROJECT_SOURCE_DIR}/Hardware/*.cpp")

set(PROJECT_SOURCE
    ${INSTRUCTIONS_SOURCE}
    ${HARDWARE_SOURCE}
    "${PROJECT_SOURCE_DIR}/main.cpp")

add_executable(emulator ${PROJECT_SOURCE})

target_link_libraries(emulator SDL2)