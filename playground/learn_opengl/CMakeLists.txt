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

message(STATUS "SHADER" ${shader_directory})
add_custom_target(run_spinning_cube
    COMMAND spinning_cube "${shader_directory}"
)

add_executable(wave_of_cubes wave_of_cubes.cpp)
target_link_libraries(
    wave_of_cubes
    PRIVATE glfw
    PRIVATE glad
    PRIVATE OpenGL::GL
    PRIVATE glm::glm-header-only
    PRIVATE ShaderProgram
    PRIVATE CubeModel
    )

add_custom_target(run_wave_of_cubes
    COMMAND wave_of_cubes "${shader_directory}"
)