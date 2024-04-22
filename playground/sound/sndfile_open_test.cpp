#include <sndfile.h>

#include <iostream>

int main(int argc, char* argv[]) {
    // Check correct # of args
    if (argc != 2) {
        // argv[0] is the program name
        std::cout << "Usage: " << argv[0] << " <file_name.wav>" << std::endl;
        return -1;
    }

    const char* filename = argv[1];

    SF_INFO sfinfo;
    SNDFILE* wavFile = sf_open(filename, SFM_READ, &sfinfo);

    if (wavFile == nullptr) {
        std::cout << "Error in sf_open: " << sf_strerror(wavFile) << std::endl;
        return -1;
    }

    std::cout << "Successfully opened " << filename << std::endl
    << " frames: " << sfinfo.frames << std::endl
    << " samplerate: " << sfinfo.samplerate << std::endl
    << " channels: " << sfinfo.channels << std::endl
    << " format: 0x" << std::hex << sfinfo.format << std::dec << std::endl
    << " sections: " << sfinfo.sections << std::endl
    << " seekable: " << sfinfo.seekable << std::endl
    << " Artist: " << sf_get_string(wavFile, SF_STR_ARTIST) << std::endl
    << " Song Title: " << sf_get_string(wavFile, SF_STR_TITLE) << std::endl;

    sf_close(wavFile);
     
    return 0;
}