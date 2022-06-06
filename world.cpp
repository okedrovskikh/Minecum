#include "world.h"

World::World(glm::vec3 position)
{
	update(position);
}

Chunk* World::getChunk(glm::vec3 position) const
{
	for (int i = 0; i < chunk.size(); i++)
	{
		if (chunk[i] != nullptr)
			if (lyingBetween(position.x, 0.0f, chunk[i]->bottom.x - 0.5f, chunk[i]->top.x + 0.5f))
				if (lyingBetween(position.z, 0.0f, chunk[i]->bottom.z - 0.5f, chunk[i]->top.z + 0.5f))
					return chunk[i];
	}

	return nullptr;
}

std::vector<Chunk*> World::getChunks(glm::vec3 position, glm::vec3 newPosition) const
{
	std::vector<Chunk*> result;
	for (int i = 0; i < chunk.size(); i++)
	{
		if (chunk[i] != nullptr)
			if (lyingBetween(position.x, 0.3f, chunk[i]->bottom.x - 0.5f, chunk[i]->top.x + 0.5f) || lyingBetween(newPosition.x, 0.3f, chunk[i]->bottom.x - 0.5f, chunk[i]->top.x + 0.5f))
				if (lyingBetween(position.z, 0.3f, chunk[i]->bottom.z - 0.5f, chunk[i]->top.z + 0.5f) || lyingBetween(newPosition.z, 0.3f, chunk[i]->bottom.z - 0.5f, chunk[i]->top.z + 0.5f))
					result.push_back(chunk[i]);
	}

	return result;
}

void World::update(glm::vec3 position)
{
	bool done = false;
	Chunk* currChunk = getChunk(position);

	if (currChunk == nullptr) {
		currChunk = new Chunk(position);
		chunk.push_back(currChunk);
		done = true;
	}
	for (int i = 0; i < 4; i++)
	{
		if (getChunk(position + vec3a[i]) == nullptr) {
			chunk.push_back(new Chunk(currChunk->bottom + vec3a[i]));
			done = true;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (getChunk(position - vec3a[i]) == nullptr) {
			chunk.push_back(new Chunk(currChunk->bottom - vec3a[i]));
			done = true;
		}
	}

	if (done)
		updateMeshes();
}

void World::updateMeshes()
{
	for (int i = 0; i < chunk.size(); i++)
		getChunksNeighboor(i);
	for (int i = 0; i < chunk.size(); i++)
		chunk[i]->updateMesh();
}

void World::getChunksNeighboor(int i)
{
	int j = 0;
	for (; j < 2; j++)
	{
		chunk[i]->chunks[j] = getChunk(chunk[i]->bottom - vec3e[j]);
	}
	for (; j < 4; j++)
	{
		chunk[i]->chunks[j] = getChunk(chunk[i]->top + vec3e[j % 2]);
	}
}

bool World::lyingBetween(float coord, float delta, float firstBound, float secondBound) const
{
	return ((firstBound <= coord - delta || firstBound <= coord + delta) && (coord - delta <= secondBound || coord + delta <= secondBound));
}

World::~World()
{
	for (int i = 0; i < chunk.size(); i++)
		delete chunk[i];
}