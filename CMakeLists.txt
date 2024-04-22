cmake_minimum_required(VERSION 3.12)
project(CornerstoneAV VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

find_package(OpenGL REQUIRED)

# Options for glfw
# Disable tests, examples and documentation
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Options for sndfile
set(BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

# Options for portaudio
set(PA_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(PA_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(PA_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add external dependencies
add_subdirectory(deps/glfw)
add_subdirectory(deps/glad)
add_subdirectory(deps/glm)
add_subdirectory(deps/sndfile)
add_subdirectory(deps/portaudio)

# Add my modules
add_subdirectory(playground)