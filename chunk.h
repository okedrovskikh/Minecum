#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include "block.h"

const unsigned int SIZE = 6 * 6 * 16;

class Chunk
{
public:
	std::pair<glm::vec3, BlockType>* coordinate;
	Chunk(glm::vec3 position);
	~Chunk();
private:
};
