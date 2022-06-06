#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <array>

#include "block.h"

const unsigned int CHUNK_SIZE_X = 6;
const unsigned int CHUNK_SIZE_Y = 32;
const unsigned int CHUNK_SIZE_Z = 6;
const unsigned int CHUNK_SIZE = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;

class Chunk
{
public:
	glm::vec3 top;
	glm::vec3 bottom;
	BlockData* coordinate;
	std::vector<BlockData*> mesh;
	// 0 - negativeX, 1 - negativeZ, 2 - positiveX, 3 - positiveZ
	std::array<Chunk*, 4> chunks = { nullptr, nullptr, nullptr, nullptr };
	Chunk();
	Chunk(glm::vec3 position);
	int getBlockIndex(glm::vec3 position);
	void updateMesh();
	~Chunk();
private:
	std::array<int, CHUNK_SIZE_X* CHUNK_SIZE_Z> createChunkHeightMap(const glm::vec3 position);
};
