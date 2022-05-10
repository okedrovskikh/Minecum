#pragma once
#include "shader.h"

class Crosshair
{
public:
    unsigned int VAO;
    unsigned int VBO;
    Shader* shader;
	Crosshair(std::string vertexPath, std::string fragmentPath);
    void draw();
	~Crosshair();
private:
    float vertices[288] = {
        -0.02f, -0.0025f, 0.0f, 0.5f, 0.5f, 0.5f,
        0.02f, -0.0025f, 0.0f, 0.5f, 0.5f, 0.5f,
        0.02f, 0.0025f, 0.0f, 0.5f, 0.5f, 0.5f,
        0.02f, 0.0025f, 0.0f, 0.5f, 0.5f, 0.5f,
        -0.02f, 0.0025f, 0.0f, 0.5f, 0.5f, 0.5f,
        -0.02f, -0.0025f, 0.0f, 0.5f, 0.5f, 0.5f,

        -0.0015f, -0.035f, 0.0f, 0.5f, 0.5f, 0.5f,
        0.0015f, -0.035f, 0.0f, 0.5f, 0.5f, 0.5f,
        0.0015f, 0.035f, 0.0f, 0.5f, 0.5f, 0.5f,
        0.0015f, 0.035f, 0.0f, 0.5f, 0.5f, 0.5f,
        -0.0015f, 0.035f, 0.0f, 0.5f, 0.5f, 0.5f,
        -0.0015f, -0.035f, 0.0f, 0.5f, 0.5f, 0.5f,
    };
};