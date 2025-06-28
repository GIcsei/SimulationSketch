# CMakeList.txt : CMake project for evolution_sim, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.16)
project (evolution_sim VERSION 0.1 LANGUAGES CXX)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


message(STATUS "Configuring project: ${PROJECT_NAME}")
message(STATUS "Source directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")

# Set C++ standard for the project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to enable CUDA
option(WITH_CUDA "Enable CUDA support" OFF)

# Enable CUDA language if requested
if(WITH_CUDA)
    message(STATUS "CUDA support ENABLED")
    enable_language(CUDA)
endif()


# ------------------------------------------------------------------------------
# Dependencies via FetchContent
# ------------------------------------------------------------------------------

# FetchContent for dependencies
include(FetchContent)

message(STATUS "Fetching dependencies...")
FetchContent_Declare(
        imgui-glfw-glad-glm
        GIT_REPOSITORY https://github.com/cmmw/imgui-glfw-glad-glm.git
        GIT_TAG master
)

FetchContent_MakeAvailable(imgui-glfw-glad-glm)

message(STATUS "GLFW fetched")
message(STATUS "ImGui fetched")
# ------------------------------------------------------------------------------
# Source Files
# ------------------------------------------------------------------------------
message(STATUS "Collecting source files...")
# Globbing all .cpp and .h files recursively
file(GLOB_RECURSE SRC_CPP "src/*.cpp")
file(GLOB_RECURSE SRC_H   "src/*.h")

set(SOURCES ${SRC_CPP} ${SRC_H})
message(STATUS "Found source files: ${SRC_CPP}")
message(STATUS "Found header files: ${SRC_H}")

# If CUDA is enabled, include CUDA files
if(WITH_CUDA)
    file(GLOB_RECURSE SRC_CU "src/*.cu")
    list(APPEND SOURCES ${SRC_CU})
    message(STATUS "Found CUDA sources: ${SRC_CU}")
endif()

# ------------------------------------------------------------------------------
# Executable
# ------------------------------------------------------------------------------

add_executable(evolution_sim ${SOURCES} "src/renderer/Shaders.h" "src/simulation/Grid.h") 
message(STATUS "Executable target created: evolution_sim")
# ------------------------------------------------------------------------------
# Include directories
# ------------------------------------------------------------------------------

# Add src and all its subdirectories
file(GLOB_RECURSE HEADER_DIRS LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/src/*")
foreach(dir ${HEADER_DIRS})
    if(IS_DIRECTORY ${dir})
        target_include_directories(evolution_sim PRIVATE ${dir})
    endif()
endforeach()

target_include_directories(evolution_sim PRIVATE
    ${PROJECT_SOURCE_DIR}/src    
)
message(STATUS "Added include directory: ${CMAKE_CURRENT_SOURCE_DIR}/src")

# ------------------------------------------------------------------------------
# Libraries
# ------------------------------------------------------------------------------
find_package(Threads REQUIRED)


target_link_libraries(evolution_sim PRIVATE
    imgui
    glm
    Threads::Threads
)

# CUDA linking
if(WITH_CUDA)
    find_package(CUDAToolkit REQUIRED)
    set_target_properties(evolution_sim PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    target_link_libraries(evolution_sim PRIVATE CUDA::cudart)
    target_compile_definitions(evolution_sim PRIVATE WITH_CUDA=1)
endif()

# ------------------------------------------------------------------------------
# Warnings
# ------------------------------------------------------------------------------
# ---- Windows Definitions (fix OpenGL APIENTRY issues) ----
if (WIN32)
    target_compile_definitions(evolution_sim PRIVATE
        NOMINMAX
        UNICODE
        _UNICODE
        WIN32_LEAN_AND_MEAN
        GLFW_INCLUDE_NONE # prevent GLFW from including <GL/gl.h>
    )
endif()

# Enable warnings
if(MSVC)
    target_compile_options(evolution_sim PRIVATE /W4 /permissive-)
else()
    target_compile_options(evolution_sim PRIVATE -Wall -Wextra -Wpedantic)
endif()
