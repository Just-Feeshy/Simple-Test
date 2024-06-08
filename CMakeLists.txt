cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

project(basic)
enable_language(CXX)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug$<$<CONFIG:Debug>:Debug>")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
endif()

file(GLOB_RECURSE SOURCES "project/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
