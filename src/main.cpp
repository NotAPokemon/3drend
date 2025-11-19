#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vector.h"
#include <functional>
#include <vector>
#include "functional_utils.h"
#include "shape.h"

Task mainTask = Task();

const char *vertexShaderSrc = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uMVP;

void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
}

)";

const char *fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;

uniform vec4 uColor;

void main() {
    FragColor = uColor;
}


)";

GLuint init_shaders()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vert);

    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(frag);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vert);
    glAttachShader(shaderProgram, frag);
    glLinkProgram(shaderProgram);

    glDeleteShader(vert);
    glDeleteShader(frag);

    return shaderProgram;
}

int main()
{
    bool init_done = false;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    init_done = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(800, 800, "Walk around the room", NULL, NULL);

    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    GLuint shaderProgram = init_shaders();

    World &world = World::getInstance();

    world.setWorldSize(Vector3(50, 50, 50));

    Shape square(window, shaderProgram);
    square.square(5);
    square.setColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
    square.translate(Vector3(0.5f, 0.5f, 0.0f));
    world.bindShape("player", &square);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        square.draw();

        Shape *player = world.getShape("player");
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            player->translate(Vector3(0.0f, 0.15f, 0.0f));
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            player->translate(Vector3(0.0f, -0.15f, 0.0f));
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            player->translate(Vector3(-0.15f, 0.0f, 0.0f));
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            player->translate(Vector3(0.15f, 0.0f, 0.0f));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}