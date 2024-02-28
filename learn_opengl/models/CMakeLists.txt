add_library(
    CubeModel STATIC
    cube_model.cpp
    cube_model.h
)

target_include_directories(CubeModel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})