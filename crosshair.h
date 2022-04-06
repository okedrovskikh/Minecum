#pragma once
#include "shader.h"
#include "shader.h"
#include "texture.h"


class Crosshair
{
public:
	Crosshair(unsigned int& VAO, unsigned int& VBO);
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