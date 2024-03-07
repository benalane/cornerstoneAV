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

# Move shader files into directory
file(GLOB input_files "${CMAKE_CURRENT_SOURCE_DIR}/inputs/*")
foreach(input_file IN LISTS input_files)
    configure_file("${input_file}"
        "${CMAKE_CURRENT_BINARY_DIR}"
        COPYONLY)
endforeach()
