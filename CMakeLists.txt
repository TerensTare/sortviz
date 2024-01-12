
cmake_minimum_required(VERSION 3.25.0)

project(
    sortviz
    VERSION 0.1.0
    DESCRIPTION "A program that shows how common sorting algorithms work."
    LANGUAGES CXX
)

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)

if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "CMake should not be used to build sortviz in a directory that has a CMakeLists.txt file!!\n
        Please consider making a \"build\" subdirectory and run cmake .. from the \"build\" subdirectory.")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ########################
# Find the dependencies #
# ########################
find_package(imgui CONFIG REQUIRED)

find_package(SDL2 CONFIG REQUIRED)

add_executable(sortviz main.cpp)

target_link_libraries(
    sortviz
    PUBLIC
    imgui::imgui

    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

target_compile_features(
    sortviz
    PUBLIC
    cxx_std_20
)