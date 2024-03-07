#include <sndfile.h>
#include <portaudio.h>

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


    sf_close(wavFile);
     
    return 0;
}