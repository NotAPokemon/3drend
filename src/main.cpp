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

void init_scene(GLFWwindow *window, GLuint shaderProgram)
{
    World &world = World::getInstance();
    Shape *square = new Shape(window, shaderProgram);
    square->square(5);
    square->setColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
    square->translate(Vector3(0.5f, 0.5f, 0.0f));
    Shape *head = new Shape(window, shaderProgram);
    head->triangle_of(5.0f, 5.0f, 5.0f);
    head->translate(Vector3(0.5f, 5.5f, 0.0f));
    Shape *boor_door = new Shape(window, shaderProgram);
    boor_door->rectangle(2, 3);
    boor_door->setColor(Vector4(0.5f, 0.25f, 0.0f, 1.0f));
    boor_door->translate(Vector3(2.0f, 0.5f, 0.0f));
    CompoundShape *character = CompoundShape::bindShapes({square, head, boor_door});
    Shape *hexagon = new Shape(window, shaderProgram);
    hexagon->setColor(Vector4(0.0f, 0.4f, 1.0f, 1.0f));
    hexagon->regularPolygon(6, 3.0f);
    hexagon->translate(Vector3(20.0f, 20.0f, 0.0f));

    world.bindShape("hexagon", hexagon);
    world.bindShape("player", character);

    delete square;
    delete head;
    delete boor_door;
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

    init_scene(window, shaderProgram);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        world.drawAllShapes();
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