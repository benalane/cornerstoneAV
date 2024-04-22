#include <portaudio.h>
#include <sndfile.h>

#include <iostream>

static int
output_cb(const void* input,
          void* output,
          unsigned long frames_per_buffer,
          const PaStreamCallbackTimeInfo* time_info,
          PaStreamCallbackFlags flags,
          void* data) {
    SNDFILE* file = (SNDFILE*)data;

    // this should not actually be done inside of the stream callback
    // but in an own working thread
    sf_count_t readCount = sf_readf_short(file, (short*)output, frames_per_buffer);

    if (readCount == 0) {
        return paComplete;
    }

    return paContinue;
}

int portAudioError(const PaError& err) {
    Pa_Terminate();
    std::cerr << "An error occurred while using the portaudio stream" << std::endl
              << "Error number: " << err << std::endl
              << "Error message: " << Pa_GetErrorText(err) << std::endl;
    return -1;
}

int main(int argc, char* argv[]) {
    // Check correct # of args
    if (argc != 2) {
        // argv[0] is the program name
        std::cerr << "Usage: " << argv[0] << " <file_name.wav>" << std::endl;
        return -1;
    }

    const char* filename = argv[1];

    SF_INFO sfinfo;
    SNDFILE* wavFile = sf_open(filename, SFM_READ, &sfinfo);

    if (wavFile == nullptr) {
        std::cerr << "Error in sf_open: " << sf_strerror(wavFile) << std::endl;
        return -1;
    }

    PaStreamParameters outputParameters;
    PaStream* stream;
    PaError err;

    err = Pa_Initialize();
    if (err != paNoError) {
        return portAudioError(err);
    }

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        std::cerr << "Haven't found an audio device!" << std::endl;
        return -1;
    }

    outputParameters.channelCount = sfinfo.channels;
    // TODO: get this from sfinfo.format
    outputParameters.sampleFormat = paInt16;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;

    err = Pa_OpenStream(&stream, NULL, &outputParameters, sfinfo.samplerate,
                        paFramesPerBufferUnspecified, paNoFlag,
                        output_cb, wavFile);
    if (err != paNoError) {
        return portAudioError(err);
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        return portAudioError(err);
    }

    while (Pa_IsStreamActive(stream) == 1) {
        Pa_Sleep(10);
    }

    err = Pa_StopStream(stream);
    if (err != paNoError) {
        return portAudioError(err);
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        return portAudioError(err);
    }

    Pa_Terminate();

    sf_close(wavFile);

    return 0;
}