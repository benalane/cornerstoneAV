#include <glad/glad.h>
// glad.h must be before glfw3.h
#include <GLFW/glfw3.h>
#include <portaudio.h>
#include <sndfile.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <climits>

#include "ShaderProgram.h"
#include "cube_model.h"

// settings
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int portAudioError(const PaError& err) {
    Pa_Terminate();
    std::cerr << "An error occurred while using the portaudio stream" << std::endl
              << "Error number: " << err << std::endl
              << "Error message: " << Pa_GetErrorText(err) << std::endl;
    return -1;
}

void processInput(GLFWwindow* window, PaStream* stream) {
    bool musicStopped = (Pa_IsStreamActive(stream) != 1);
    bool windowShouldClose = (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS);

    if (musicStopped || windowShouldClose) {
        glfwSetWindowShouldClose(window, true);

        portAudioError(Pa_StopStream(stream));
    }
}

// throwing this together with a global
short left = 0;
short right = 0;

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
    left = ((short*)output)[0];
    right = ((short*)output)[0];

    if (readCount == 0) {
        return paComplete;
    }

    return paContinue;
}

int main(int argc, char* argv[]) {
    // Check correct # of args
    if (argc != 3) {
        // argv[0] is the program name
        std::cerr << "Usage: " << argv[0] << " <shader_directory> <file_name.wav>" << std::endl;
        return -1;
    }
    std::string shaderDirectory = argv[1];
    const char* filename = argv[2];

    glfwInit();

    // Use OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Explicitly use core profile, no need for backwards compatability.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Needed for MacOS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a window object. Args: width, height, name, ...
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Male the window the main context on current thread
    glfwMakeContextCurrent(window);

    // Anytime the user reseizes the window, a callback function is called.
    // We use that to also adjust the viewport.
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Initialize GLAD before we call any OpenGL functions.
    // glfwGetProcAddress defines the correct function pointers based on which OS we're compiling for
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Globals
    glEnable(GL_DEPTH_TEST);

    std::string vertexShaderSource = shaderDirectory + "/projection.vs";
    std::string fragmentShaderSource = shaderDirectory + "/passthru.fs";
    ShaderProgram projectionShader(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

    // Generate vertex array object
    unsigned int vao;
    glGenVertexArrays(1, &vao);

    // Generate vertex buffer object
    unsigned int vbo;
    glGenBuffers(1, &vbo);

    unsigned int ebo;
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Copy vertices from first triangle into first vbo
    glBufferData(GL_ARRAY_BUFFER, getCubeVerticesSize(), getCubeVertices(), GL_STATIC_DRAW);
    // Instruct OpenGL how to interpret vertex data in vbo, and enable

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getCubeElementIndexesSize(), getCubeElementIndexes(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    float scaleX = 0.25;
    float scaleZ = 0.25;
    float decay = 0.993;
    constexpr unsigned int numBars = 1000;
    constexpr unsigned int delay = 1;

    std::vector<float> heights;
    heights.resize(numBars * delay);

    std::vector<unsigned int> heightIndices;
    heightIndices.resize(numBars);

    std::vector<float> decayFactors;
    decayFactors.resize(numBars);

    // Initial indices and rates;
    heightIndices[0] = 0;
    decayFactors[0] = 1;
    for (int i = 1; i < heightIndices.size(); i++) {
        heightIndices[i] = delay * (heightIndices.size() - i);
        decayFactors[i] = decay * decayFactors[i - 1];
    }

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

    // Render loop.
    // glfwWindowShouldClose checks if the window has been instruted to close
    while (!glfwWindowShouldClose(window)) {
        processInput(window, stream);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // glClearColor function is a state-setting function and glClear is a state-using function in that it uses the current state to retrieve the clearing color from.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // All options: GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT

        projectionShader.use();

        // create transformations
        glm::mat4 view = glm::mat4(1.0f);  // make sure to initialize matrix to identity matrix first
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        view = glm::rotate(view, 0.5f, glm::vec3(0.25f, -1.0f, 0.0f));

        // pass transformation matrices to the shader
        projectionShader.setMat4("projection", projection);  // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        projectionShader.setMat4("view", view);

        // float cTime = glfwGetTime();
        // heights[heightIndices[0]] = abs(sin(cTime) + sin(cTime * 3) + sin(cTime * 0.7) + sin(cTime * 2.1));
        float noramlizedSample = ((0.25 * noramlizedSample) + (((float)(left)) / (SHRT_MAX/4)))/2; 
        heights[heightIndices[0]] = abs(noramlizedSample);

        for (int i = 0; i < numBars; ++i) {
            float height = heights[heightIndices[i]] * decayFactors[i];

            heightIndices[i] = heightIndices[i] + 1;
            if (heightIndices[i] >= heights.size()) {
                heightIndices[i] = 0;
            }

            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            glm::vec3 position = glm::vec3(0.0f, -1.0 + (0.5 * height), -1 * i * scaleZ);
            model = glm::translate(model, position);
            model = glm::scale(model, glm::vec3(scaleX, height, scaleZ));
            projectionShader.setMat4("model", model);
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        return portAudioError(err);
    }

    Pa_Terminate();

    sf_close(wavFile);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();

    return 0;
}