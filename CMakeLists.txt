cmake_minimum_required(VERSION 3.12)
project(CornerstoneAV VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

find_package(OpenGL REQUIRED)

# Disable GLFW tests, examples and documentation
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(deps/glfw)

add_subdirectory(deps/glad)

add_subdirectory(learn_opengl)