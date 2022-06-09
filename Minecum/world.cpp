#include "world.h"

World::World(glm::vec3 position)
{
	extendWorld(position);

	for (int i = 0; i < chunk.size(); i++)
		getChunksNeighboor(i);

	updateActiveChunks(position);

	for (int i = 0; i < chunk.size(); i++)
		chunk[i]->updateMesh();
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

bool World::extendWorld(glm::vec3 position)
{
	bool update = false;
	Chunk* currChunk = getChunk(position);

	if (currChunk == nullptr) {
		currChunk = new Chunk(position);
		chunk.push_back(currChunk);
		update = true;
	}
	for (int i = 0; i < 4; i++)
	{
		if (getChunk(position + vec3a[i]) == nullptr) {
			chunk.push_back(new Chunk(currChunk->bottom + vec3a[i]));
			update = true;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (getChunk(position - vec3a[i]) == nullptr) {
			chunk.push_back(new Chunk(currChunk->bottom - vec3a[i]));
			update = true;
		}
	}

	return update;
}

void World::updateActiveChunks(glm::vec3 position)
{
	activeChunk.clear();

	Chunk* currChunk = getChunk(position);

	activeChunk.push_back(currChunk);
	
	for (int i = 0; i < 4; i++)
	{
		activeChunk.push_back(currChunk->chunks[i]);
		if (i % 2 == 0) {
			activeChunk.push_back(currChunk->chunks[i]->chunks[1]);
			activeChunk.push_back(currChunk->chunks[i]->chunks[3]);
		}
	}
}

void World::update(glm::vec3 position)
{
	if (extendWorld(position))
		updateMesh();

	updateActiveChunks(position);
}

void World::updateMesh()
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
		if (chunk[i]->chunks[j] == nullptr)
			chunk[i]->chunks[j] = getChunk(chunk[i]->bottom - vec3e[j]);
	}
	for (; j < 4; j++)
	{
		if (chunk[i]->chunks[j] == nullptr)
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