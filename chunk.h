#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>

const unsigned int SIZE = 16 * 16 * 128;

class Chunk
{
public:
	glm::vec3 coordinate[SIZE] = {
		glm::vec3(-1.0f, -0.5f, -1.0f), glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(1.0f,  -0.5f, -1.0f),
		glm::vec3(-1.0f, -0.5f, 0.0f), glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(1.0f,  -0.5f, 0.0f),
		glm::vec3(-1.0f, -0.5f, 1.0f), glm::vec3(0.0f, -0.5f, 1.0f), glm::vec3(1.0f,  -0.5f, 1.0f),
		glm::vec3(-1.0f, -1.5f, -1.0f), glm::vec3(0.0f, -1.5f, -1.0f), glm::vec3(1.0f, -1.5f, -1.0f),
		glm::vec3(-1.0f, -1.5f, 0.0f), glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(1.0f,  -1.5f, 0.0f),
		glm::vec3(-1.0f, -1.5f, 1.0f), glm::vec3(0.0f, -1.5f, 1.0f), glm::vec3(1.0f,  -1.5f, 1.0f),
		glm::vec3(-1.0f, -2.5f, -1.0f), glm::vec3(0.0f, -2.5f, -1.0f), glm::vec3(1.0f,  -2.5f, -1.0f),
		glm::vec3(-1.0f, -2.5f, 0.0f), glm::vec3(0.0f, -2.5f, 0.0f), glm::vec3(1.0f,  0.5f, 1.0f),
		glm::vec3(-1.0f, 1.5f, 1.0f), glm::vec3(0.0f, -2.5f, 1.0f), glm::vec3(1.0f,  1.5f, 1.0f),
		glm::vec3(1.0f,  2.5f, 1.0f), glm::vec3(1.0f,  3.5f, 1.0f)
	};
	Chunk();
	~Chunk();
private:
};
