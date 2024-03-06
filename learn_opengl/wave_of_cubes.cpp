#include <glad/glad.h>
// glad.h must be before glfw3.h
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "ShaderProgram.h"
#include "cube_model.h"

// settings
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

// Callback for when the framebuffer is resized. We set the viewport again.
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

int main() {
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

    // Tell OpenGL the size of the rendering window.
    // lower left corner x/y and width/height
    // int frameBufferWidth, frameBufferHeight;
    // glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    // glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    // Globals
    glEnable(GL_DEPTH_TEST);

    ShaderProgram projectionShader("shaders/projection.vs", "shaders/passthru.fs");

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

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    float scale = 0.25;
    float decay = 0.99;
    constexpr unsigned int numBars = 1000;
    constexpr unsigned int delay = 10;

    // Will move this into some OOP principles later. For now... a comment.
    // heights, heightIndices, and decayFactors allow for the wave of cubes.
    // heights acts as a circular array. Each measurement is recorded in the array
    // and loops around to the front once the array is full.
    // heightIndices holds which element of heights each box will render from
    // The indices are offset by delay so each box trails it's forward box by delay values
    // At initialization, the front box is at the head (where the first measurement will be written)
    // Each subsequent box is then offset by the delay, but must first wrap around to the end of heights
    //     with 7 boxes and a delay of 2          v--delay--v
    // | H0 | __ | H6 | __ | H5 | __ | H4 | __ | H3 | __ | H2 | __ | H1 | __ |
    // | __ | H0 | __ | H6 | __ | H5 | __ | H4 | __ | H3 | __ | H2 | __ | H1 |
    // | H1 | __ | H0 | __ | H6 | __ | H5 | __ | H4 | __ | H3 | __ | H2 | __ |
    // | __ | H1 | __ | H0 | __ | H6 | __ | H5 | __ | H4 | __ | H3 | __ | H2 |
    // The above shows init and 3 subsequen iterations, showing where the indices point ot
    // Each new value is writted at H0, H0 is then rendered and incremented
    // Each subsequent box is rendered and its index is incremented
    // For rendereing, each box is scaled by a decay factor stored in decayFactors
    // h = a * (1 - r)^t, storing the decay as (1 - r) means that each subsequent box's
    // decay factor is just the previous box's multiplied by the factor.


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

    // Render loop.
    // glfwWindowShouldClose checks if the window has been instruted to close
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

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
        view = glm::rotate(view, 0.5f, glm::vec3(0.25f, 1.0f, 0.0f));

        // pass transformation matrices to the shader
        projectionShader.setMat4("projection", projection);  // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        projectionShader.setMat4("view", view);

        float cTime = glfwGetTime();
        heights[heightIndices[0]] = abs(sin(cTime) + sin(cTime * 3) + sin(cTime * 0.7) + sin(cTime * 2.1));

        for (int i = 0; i < numBars; ++i) {
            float height = heights[heightIndices[i]] * decayFactors[i];

            heightIndices[i] = heightIndices[i] + 1;
            if (heightIndices[i] >= heights.size()) {
                heightIndices[i] = 0;
            }

            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            glm::vec3 position = glm::vec3(0.0f, -1.0 + (0.5 * height), -1 * i * scale);
            model = glm::translate(model, position);
            model = glm::scale(model, glm::vec3(scale, height, scale));
            projectionShader.setMat4("model", model);
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();

    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    // glfwGetKey will return GLFW_RELEASE if escape is NOT being pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
