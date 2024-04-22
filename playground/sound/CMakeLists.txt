# Move input files into directory
set(input_directory "${PROJECT_BINARY_DIR}/inputs" CACHE INTERNAL "")
file(MAKE_DIRECTORY ${input_directory})
file(GLOB input_files "${CMAKE_CURRENT_SOURCE_DIR}/inputs/*")
foreach(input_file IN LISTS input_files)
    configure_file("${input_file}"
        "${input_directory}"
        COPYONLY)
endforeach()

add_executable(
    sndfile_open_test 
    sndfile_open_test.cpp
)
target_link_libraries(
    sndfile_open_test
    PRIVATE sndfile
)
add_custom_target(run_sndfile_open_test
    COMMAND sndfile_open_test ./redbone.wav
)

add_executable(
    portaudio_play_music_test
    portaudio_play_music_test.cpp
)
target_link_libraries(
    portaudio_play_music_test
    PRIVATE sndfile
    PRIVATE PortAudio
)
add_custom_target(run_portaudio_play_music_test
    COMMAND portaudio_play_music_test ${input_directory}/redbone.wav
)

add_executable(
    visualizer_test
    visualizer_test.cpp
)
target_link_libraries(
    visualizer_test
    PRIVATE sndfile
    PRIVATE PortAudio
    PRIVATE glfw
    PRIVATE glad
    PRIVATE OpenGL::GL
    PRIVATE glm::glm-header-only
    PRIVATE ShaderProgram
    PRIVATE CubeModel
)
add_custom_target(run_visualizer_test
    COMMAND visualizer_test ${shader_directory} ${input_directory}/redbone.wav
)
