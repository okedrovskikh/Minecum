#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include "block.h"

const unsigned int SIZE = 6 * 6 * 32;

class Chunk
{
public:
	std::pair<glm::vec3, std::pair<BlockType, bool>>* coordinate;
	Chunk();
	Chunk(glm::vec3 position);
	int getIndex(glm::vec3 position);
	~Chunk();
private:
};
