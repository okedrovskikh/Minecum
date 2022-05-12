#pragma once
#include "chunk.h"
#include <vector>

const unsigned int WORLD_SIZE_X = 4;
const unsigned int WORLD_SIZE_Z = 4;
const unsigned int WORLD_SIZE = WORLD_SIZE_X * WORLD_SIZE_Z;
const glm::vec3 START_POINT = glm::vec3(-1.0f * WORLD_SIZE_X / 2 * CHUNK_SIZE_X, 0.0f, -1.0f * (WORLD_SIZE_Z + 2) / 2 * CHUNK_SIZE_Z);

class World
{
public:
	Chunk** chunk;
	World();
	std::vector<Chunk*> getChunks(glm::vec3 position) const;
	std::vector<Chunk*> getChunks(glm::vec3 position, glm::vec3 newPosition) const;
	~World();
private:
	bool lyingBetween(float coordinate, float delta, float firstBound, float secondBound) const;
};