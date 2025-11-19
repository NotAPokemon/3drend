#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <glad/glad.h>
#include "vector.h"
#include <vector>
#include "functional_utils.h"
#include <GLFW/glfw3.h>
#include <unordered_map>

class Shape;

class World
{
private:
    Vector3 worldSize;
    std::vector<Shape *> shapes;
    std::unordered_map<std::string, Shape *> shapeNames;

public:
    World() {}
    ~World() {}
    void setWorldSize(const Vector3 &size)
    {
        worldSize = size;
    }
    Vector3 getWorldSize() const
    {
        return worldSize;
    }
    void bindShape(std::string name, Shape *shape)
    {
        shapes.push_back(shape);
        shapeNames[name] = shape;
    }

    Shape *getShape(std::string name)
    {
        return shapeNames[name];
    }

    static World &getInstance()
    {
        static World instance;
        return instance;
    }

    static bool isBound(Shape *shape)
    {
        World &world = getInstance();
        for (auto s : world.shapes)
        {
            if (s == shape)
            {
                return true;
            }
        }
        return false;
    }

    static bool isBound(std::string name)
    {
        World &world = getInstance();
        return world.shapeNames.find(name) != world.shapeNames.end();
    }
};

class Shape
{
private:
    std::vector<Vector3> vertices;

    unsigned int Vertex_Array_Object, Vertex_Buffer_Object;
    Vector4 color;
    GLuint shader;
    GLFWwindow *window;
    bool initialized = false;

    Mat4 model;

    void init()
    {
        glGenVertexArrays(1, &Vertex_Array_Object); // Create VAO
        glGenBuffers(1, &Vertex_Buffer_Object);     // Create VBO

        glBindVertexArray(Vertex_Array_Object); // register VAO as current

        glBindBuffer(GL_ARRAY_BUFFER, Vertex_Buffer_Object);                                               // register VBO as current
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), vertices.data(), GL_STATIC_DRAW); // fill buffer with vertex data

        glVertexAttribPointer(0 /*the shader location*/,
                              3 /*Vertex size*/,
                              GL_FLOAT /*data type*/,
                              GL_FALSE /*Tell glad not to normalize the vectors*/,
                              sizeof(Vector3) /*Distance between bytes */,
                              (void *)0 /*Byte offset */); // GPU configuration for vertex drawing

        glEnableVertexAttribArray(0); // enable shader location 0

        glBindBuffer(GL_ARRAY_BUFFER, 0); // clear VBO context
        glBindVertexArray(0);             // clear VAO context
    }

public:
    Shape(GLFWwindow *window, GLuint shader)
    {
        this->window = window;
        this->shader = shader;
        model.loadIdentity();
    }
    ~Shape() {}
    void addVertex(const Vector3 &v1)
    {
        vertices.push_back(v1);
        initialized = false;
    }
    void setVertex(int index, const Vector3 &v)
    {
        if (index >= 0 && index < vertices.size())
        {
            vertices[index] = v;
        }
        else
        {
            vertices.push_back(v);
        }
        initialized = false;
    }

    void clearVertices()
    {
        vertices.clear();
        initialized = false;
    }

    void setColor(const Vector4 &c)
    {
        color = c;
        initialized = false;
    }

    void triangle_of(float a, float b, float c)
    {
        clearVertices();

        Vector3 A(0, 0, 0);
        Vector3 B(c, 0, 0);

        float x = (b * b + c * c - a * a) / (2 * c);
        float y = sqrt(b * b - x * x);

        Vector3 C(x, y, 0);

        addVertex(A);
        addVertex(B);
        addVertex(C);
        initialized = false;
    }

    void square(float size)
    {
        clearVertices();

        Vector3 A(0, 0, 0);
        Vector3 B(size, 0, 0);
        Vector3 C(size, size, 0);
        Vector3 D(0, size, 0);

        addVertex(A);
        addVertex(B);
        addVertex(C);

        addVertex(A);
        addVertex(C);
        addVertex(D);
        initialized = false;
    }

    void translate(const Vector3 &p)
    {
        for (auto &v : vertices)
        {
            v = v + p;
        }
        initialized = false;
    }

    void setPos(const Vector3 &p)
    {
        Vector3 v1 = vertices.at(0);
        std::vector<Vector3> offsets;
        for (const auto &v : vertices)
        {
            offsets.push_back(v - v1);
        }
        clearVertices();
        for (const auto &offset : offsets)
        {
            addVertex(p + offset);
        }
        initialized = false;
    }

    void setScale(float s)
    {
        for (auto &v : vertices)
        {
            v = v * s;
        }
        initialized = false;
    }

    void draw()
    {
        if (!World::isBound(this))
        {
            return;
        }
        if (!initialized)
        {
            init();
            initialized = true;
        }

        glBindVertexArray(Vertex_Array_Object); // register VAO as current
        GLuint colorLoc = glGetUniformLocation(shader, "uColor");
        glUniform4f(colorLoc, color.x, color.y, color.z, color.w);

        World &world = World::getInstance();
        Vector3 worldSize = world.getWorldSize();

        Mat4 proj = Mat4::ortho(0.0f, worldSize.x, 0.0f, worldSize.y, -1.0f, 1.0f);
        Mat4 view;
        view.loadIdentity();

        Mat4 MVP = proj * view * model;
        GLuint mvpLoc = glGetUniformLocation(shader, "uMVP");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, MVP.m);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // draw the vertexs in triangle mode
        glBindVertexArray(0);                           // clear VAO context
    }
};
#endif