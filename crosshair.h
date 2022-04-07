#pragma once
#include "shader.h"


class Crosshair
{
public:
    Shader shader;
	Crosshair(unsigned int& VAO, unsigned int& VBO, Shader& shader);
	~Crosshair();
private:
    float vertices[288] = {
        -0.02f, -0.005f, 0.0f, 0.5f, 0.5f, 0.5f,
        0.02f, -0.005f, 0.0f, 0.5f, 0.5f, 0.5f,
        0.02f, 0.005f, 0.0f, 0.5f, 0.5f, 0.5f,
        0.02f, 0.005f, 0.0f, 0.5f, 0.5f, 0.5f,
        -0.02f, 0.005f, 0.0f, 0.5f, 0.5f, 0.5f,
        -0.02f, -0.005f, 0.0f, 0.5f, 0.5f, 0.5f,

        -0.003f, -0.035f, 0.0f, 0.5f, 0.5f, 0.5f,
        0.003f, -0.035f, 0.0f, 0.5f, 0.5f, 0.5f,
        0.003f, 0.035f, 0.0f, 0.5f, 0.5f, 0.5f,
        0.003f, 0.035f, 0.0f, 0.5f, 0.5f, 0.5f,
        -0.003f, 0.035f, 0.0f, 0.5f, 0.5f, 0.5f,
        -0.003f, -0.035f, 0.0f, 0.5f, 0.5f, 0.5f,
    };
};