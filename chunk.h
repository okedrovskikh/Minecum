#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "block.h"

const unsigned int CHUNK_SIZE_X = 6;
const unsigned int CHUNK_SIZE_Y = 32;
const unsigned int CHUNK_SIZE_Z = 6;
const unsigned int CHUNK_SIZE = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;

class Chunk
{
public:
	std::pair<glm::vec3, BlockData>* coordinate;
	glm::vec3 top;
	glm::vec3 bottom;
	Chunk();
	Chunk(glm::vec3 position);
	int getBlockIndex(glm::vec3 position);
	~Chunk();
private:
};
