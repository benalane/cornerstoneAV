add_subdirectory(learn_opengl)
add_subdirectory(sound)

add_executable(deque deque.cpp)
add_custom_target(run_deque
    COMMAND deque
)
