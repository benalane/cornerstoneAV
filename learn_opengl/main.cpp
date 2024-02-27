#include <glad/glad.h>
// glad.h must be before glfw3.h
#include <GLFW/glfw3.h>

#include <iostream>
#include "ShaderProgram.h"

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

    ShaderProgram orangeShader("passthru.vs", "orange.fs");
    ShaderProgram yellowShader("passthru.vs", "yellow.fs");

    float triangle1[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};
    float triangle2[] = {
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        1.0f, 0.5f, 0.0f};

    // Generate vertex array objects
    unsigned int vao[2];
    glGenVertexArrays(2, vao);

    // Generate vertex buffer objects
    unsigned int vbo[2];
    glGenBuffers(2, vbo);

    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    // Copy vertices from first triangle into first vbo
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
    // Instruct OpenGL how to interpret vertex data in vbo, and enable
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    // Copy vertices from first triangle into first vbo
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
    // Instruct OpenGL how to interpret vertex data in vbo, and enable
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop.
    // glfwWindowShouldClose checks if the window has been instruted to close
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);  // All options: GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT
        // glClearColor function is a state-setting function and glClear is a state-using function in that it uses the current state to retrieve the clearing color from.

        orangeShader.use();
        glBindVertexArray(vao[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        yellowShader.use();
        glBindVertexArray(vao[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, vao);
    glDeleteBuffers(2, vbo);

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
