add_executable(cornerstoneAV main.cpp)
target_link_libraries(
    cornerstoneAV
    PRIVATE glfw
    PRIVATE glad
    PRIVATE OpenGL::GL 
    )
