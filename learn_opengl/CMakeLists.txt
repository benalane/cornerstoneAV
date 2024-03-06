add_subdirectory(models)
add_subdirectory(shaders)


add_executable(spinning_cube spinning_cube.cpp)
target_link_libraries(
    spinning_cube
    PRIVATE glfw
    PRIVATE glad
    PRIVATE OpenGL::GL
    PRIVATE glm::glm-header-only
    PRIVATE ShaderProgram
    PRIVATE CubeModel
    )

add_custom_target(run_spinning_cube
    COMMAND spinning_cube
)