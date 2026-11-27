cmake_minimum_required(VERSION 3.14)

project(Warblade LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(raylib CONFIG REQUIRED)
find_package(raylib_cpp CONFIG REQUIRED)

file(GLOB_RECURSE SRC_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

add_executable(Warblade Warblade.cpp ${SRC_HEADERS})
target_link_libraries(Warblade PRIVATE raylib raylib_cpp::raylib_cpp)

add_custom_command(TARGET Warblade POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/media"
        "$<TARGET_FILE_DIR:Warblade>/media"
)
