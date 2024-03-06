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

# Move shader files into directory
file(GLOB vertex_shader_sources "${CMAKE_CURRENT_SOURCE_DIR}/vertex/*")
file(GLOB fragment_shader_sources "${CMAKE_CURRENT_SOURCE_DIR}/fragment/*")
foreach(shader_source IN LISTS vertex_shader_sources fragment_shader_sources)
    configure_file("${shader_source}"
        "${CMAKE_CURRENT_BINARY_DIR}"
        COPYONLY)
endforeach()