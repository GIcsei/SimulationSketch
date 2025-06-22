# CMakeList.txt : CMake project for SimulationSketch, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SimulationSketch")

# Add source to this project's executable.
add_executable (SimulationSketch "./src/SimulationSketch.cpp" "./src/SimulationSketch.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SimulationSketch PROPERTY CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# TODO: Add tests and install targets if needed.
# FetchContent for dependencies
include(FetchContent)

# GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw)

# Dear ImGui
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        v1.89.8
)
FetchContent_MakeAvailable(imgui)

# Locate OpenGL
find_package(OpenGL REQUIRED)

# Source files
set(SOURCES
    src/SimulationSketch.cpp
)
if(WITH_CUDA)
    list(APPEND SOURCES
        src/simulation.cu  # your CUDA kernels
    )
endif()

# Create executable
add_executable(evolution_sim ${SOURCES})

target_include_directories(evolution_sim PRIVATE
    src
)

# Link libraries
target_link_libraries(evolution_sim PRIVATE
    glfw
    imgui
    OpenGL::GL
)

if(WITH_CUDA)
    find_package(CUDA REQUIRED)
    set_target_properties(evolution_sim PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    target_link_libraries(evolution_sim PRIVATE CUDA::cudart)
endif()

# Threading support
find_package(Threads REQUIRED)
target_link_libraries(evolution_sim PRIVATE Threads::Threads)

# Enable warnings
if(MSVC)
    target_compile_options(evolution_sim PRIVATE /W4 /permissive-)
else()
    target_compile_options(evolution_sim PRIVATE -Wall -Wextra -Wpedantic)
endif()
