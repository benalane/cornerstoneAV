add_subdirectory(models)

add_library(
    ShaderProgram STATIC
    ShaderProgram.cpp
    ShaderProgram.h
)
target_link_libraries(
    ShaderProgram
    PRIVATE glad
    PRIVATE glm::glm-header-only
)
target_include_directories(ShaderProgram PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Move shader files into binary directory.
file(GLOB shader_sources "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*")
message(STATUS "Files are ${SHADER_SOURCES}")
foreach(shader_source IN LISTS shader_sources)
    configure_file("${shader_source}"
        "${CMAKE_CURRENT_BINARY_DIR}"
        COPYONLY)
endforeach()

add_executable(learn_opengl main.cpp)
target_link_libraries(
    learn_opengl
    PRIVATE glfw
    PRIVATE glad
    PRIVATE OpenGL::GL
    PRIVATE glm::glm-header-only
    PRIVATE ShaderProgram
    PRIVATE CubeModel
    )

add_custom_target(run_learn_opengl
    COMMAND learn_opengl
)