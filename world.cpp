#include "world.h"

World::World()
{
	chunk = new Chunk*[WORLD_SIZE];
	glm::vec3 chunkPoint = START_POINT;
	for (int countX = 0; countX < WORLD_SIZE; countX++)
	{
		if (countX % WORLD_SIZE_X == 0) {
			chunkPoint.z += CHUNK_SIZE_Z;
			chunkPoint.x = START_POINT.x;
		}
		else {
			chunkPoint.x += CHUNK_SIZE_X;
		}
		chunk[countX] = new Chunk(chunkPoint);
	}

	for (int i = 0; i < WORLD_SIZE; i++)
	{
		getChunksNeighboor(i);
	}

	for (int i = 0; i < WORLD_SIZE; i++)
	{
		chunk[i]->updateMesh();
	}
}

Chunk* World::getChunk(glm::vec3 position) const
{
	for (int i = 0; i < WORLD_SIZE; i++)
	{
		if (lyingBetween(position.x, 0.3f, chunk[i]->bottom.x - 0.5f, chunk[i]->top.x + 0.5f) &&
			lyingBetween(position.z, 0.3f, chunk[i]->bottom.z - 0.5f, chunk[i]->top.z + 0.5f))
				return chunk[i];
	}

	return nullptr;
}

std::vector<Chunk*> World::getChunks(glm::vec3 position, glm::vec3 newPosition) const
{
	std::vector<Chunk*> result;
	for (int i = 0; i < WORLD_SIZE; i++)
	{
		if (lyingBetween(position.x, 0.3f, chunk[i]->bottom.x - 0.5f, chunk[i]->top.x + 0.5f) || lyingBetween(newPosition.x, 0.3f, chunk[i]->bottom.x - 0.5f, chunk[i]->top.x + 0.5f))
			if (lyingBetween(position.z, 0.3f, chunk[i]->bottom.z - 0.5f, chunk[i]->top.z + 0.5f) || lyingBetween(newPosition.z, 0.3f, chunk[i]->bottom.z - 0.5f, chunk[i]->top.z + 0.5f))
				result.push_back(chunk[i]);
	}

	return result;
}

void World::getChunksNeighboor(int i)
{
	for (int j = 0; j < 4; j++)
	{
		chunk[i]->chunks[j] = getChunk(chunk[i]->bottom - vectors[j]);
	}
	//chunk[i]->updateMesh();
}

bool World::lyingBetween(float coord, float delta, float firstBound, float secondBound) const
{
	return ((firstBound <= coord - delta || firstBound <= coord + delta) && (coord - delta <= secondBound || coord + delta <= secondBound));
}

World::~World()
{
	for (int i = 0; i < WORLD_SIZE; i++)
		delete chunk[i];
	delete[] chunk;
}