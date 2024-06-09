cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

project(basic)
enable_language(CXX)

# Set C++ Version
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Setters
SET(WINRT FALSE)
SET(ANGLE FALSE)
SET(APPLE_PLATFORM MACOSX)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
include_directories(${OPENGL_INCLUDE_DIR})

add_subdirectory(third_party/SDL)

# Just Stuff
add_compile_definitions(HAS_AUDIO)

if(WIN32)
    add_compile_definitions(WINDOWS)

	if(WINRT)
		add_compile_definitions(WINRT)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /await")
	endif()
elseif(APPLE)
    add_compile_definitions(${APPLE_PLATFORM})
elseif(UNIX)
    add_compile_definitions(LINUX)
endif()

if(ANGLE)
    add_compile_definitions(NATIVE_TOOLKIT_SDL_ANGLE)
endif()

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug$<$<CONFIG:Debug>:Debug>")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
endif()

file(GLOB SOURCES "src/RenderEvent.cpp" "project/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} SDL2-static OpenGL::GL)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE third_party/SDL/include)
