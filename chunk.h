#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "block_prototype.h"

const unsigned int SIZE = 6 * 6 * 64;

class Chunk
{
public:
	std::vector<glm::vec3> coordinate = {
		glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f,  -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f,  -1.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 1.0f), glm::vec3(1.0f,  -1.0f, 1.0f),
		glm::vec3(-1.0f, -2.0f, -1.0f), glm::vec3(0.0f, -2.0f, -1.0f), glm::vec3(1.0f, -2.0f, -1.0f),
		glm::vec3(-1.0f, -2.0f, 0.0f), glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(1.0f,  -2.0f, 0.0f),
		glm::vec3(-1.0f, -2.0f, 1.0f), glm::vec3(0.0f, -2.0f, 1.0f), glm::vec3(1.0f,  -2.0f, 1.0f),
		glm::vec3(-1.0f, -3.0f, -1.0f), glm::vec3(0.0f, -3.0f, -1.0f), glm::vec3(1.0f,  -3.0f, -1.0f),
		glm::vec3(-1.0f, -3.0f, 0.0f), glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(1.0f,  0.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -3.0f, 1.0f), glm::vec3(1.0f,  1.0f, 1.0f),
		glm::vec3(1.0f,  2.0f, 1.0f), glm::vec3(1.0f,  3.0f, 1.0f)
	};
	Chunk();
	~Chunk();
private:
};
