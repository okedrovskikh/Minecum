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

	std::cout << "#2 || " << chunk[2] << "   " << "#5 || " << chunk[5] << "   " << "# 8 || " << chunk[8] << "\n";
	std::cout << chunk[2]->top.x << " " << chunk[2]->top.y << " " << chunk[2]->top.z << "                  ";
	std::cout << chunk[5]->top.x << " " << chunk[5]->top.y << " " << chunk[5]->top.z << "                  ";
	std::cout << chunk[8]->top.x << " " << chunk[8]->top.y << " " << chunk[8]->top.z << "                  " << "\n";
	std::cout << chunk[2]->bottom.x << " " << chunk[2]->bottom.y << " " << chunk[2]->bottom.z << "                  ";
	std::cout << chunk[5]->bottom.x << " " << chunk[5]->bottom.y << " " << chunk[5]->bottom.z << "                  ";
	std::cout << chunk[8]->bottom.x << " " << chunk[8]->bottom.y << " " << chunk[8]->bottom.z << "                  " << "\n";
	for (int k = 0; k < 4; k++)
	{
		if (chunk[2]->chunks[k] != nullptr)
			std::cout << k << ") " << chunk[2]->chunks[k] << "      ";
		else
			std::cout << k << ") " << "                      ";
		if (chunk[5]->chunks[k] != nullptr)
			std::cout << k << ") " << chunk[5]->chunks[k] << "      ";
		else
			std::cout << k << ") " << "                      ";
		if (chunk[8]->chunks[k] != nullptr)
			std::cout << k << ") " << chunk[8]->chunks[k] << "      ";
		else
			std::cout << k << ") " << "                      ";
		std::cout << "\n";
	}
	std::cout << "#1 || " << chunk[1] << "   " << "#4 || " << chunk[4] << "   " << "# 7 || " << chunk[7] << "\n";
	std::cout << chunk[1]->top.x << " " << chunk[1]->top.y << " " << chunk[1]->top.z << "                  ";
	std::cout << chunk[4]->top.x << " " << chunk[4]->top.y << " " << chunk[4]->top.z << "                  ";
	std::cout << chunk[7]->top.x << " " << chunk[7]->top.y << " " << chunk[7]->top.z << "                  " << "\n";
	std::cout << chunk[1]->bottom.x << " " << chunk[1]->bottom.y << " " << chunk[1]->bottom.z << "                  ";
	std::cout << chunk[4]->bottom.x << " " << chunk[4]->bottom.y << " " << chunk[4]->bottom.z << "                  ";
	std::cout << chunk[7]->bottom.x << " " << chunk[7]->bottom.y << " " << chunk[7]->bottom.z << "                  " << "\n";
	for (int k = 0; k < 4; k++)
	{
		if (chunk[1]->chunks[k] != nullptr)
			std::cout << k << ") " << chunk[1]->chunks[k] << "      ";
		else
			std::cout << k << ") " << "                      ";
		if (chunk[4]->chunks[k] != nullptr)
			std::cout << k << ") " << chunk[4]->chunks[k] << "      ";
		else
			std::cout << k << ") " << "                      ";
		if (chunk[7]->chunks[k] != nullptr)
			std::cout << k << ") " << chunk[7]->chunks[k] << "      ";
		else
			std::cout << k << ") " << "                      ";
		std::cout << "\n";
	}
	std::cout << "#0 || " << chunk[0] << "   " << "#3 || " << chunk[3] << "   " << "# 6 || " << chunk[6] << "\n";
	std::cout << chunk[0]->top.x << " " << chunk[0]->top.y << " " << chunk[0]->top.z << "                  ";
	std::cout << chunk[3]->top.x << " " << chunk[3]->top.y << " " << chunk[3]->top.z << "                  ";
	std::cout << chunk[6]->top.x << " " << chunk[6]->top.y << " " << chunk[6]->top.z << "                  " << "\n";
	std::cout << chunk[0]->bottom.x << " " << chunk[0]->bottom.y << " " << chunk[0]->bottom.z << "                  ";
	std::cout << chunk[3]->bottom.x << " " << chunk[3]->bottom.y << " " << chunk[3]->bottom.z << "                  ";
	std::cout << chunk[6]->bottom.x << " " << chunk[6]->bottom.y << " " << chunk[6]->bottom.z << "                  " << "\n";
	for (int k = 0; k < 4; k++)
	{
		if (chunk[0]->chunks[k] != nullptr)
			std::cout << k << ") " << chunk[0]->chunks[k] << "      ";
		else
			std::cout << k << ") " << "                      ";
		if (chunk[3]->chunks[k] != nullptr)
			std::cout << k << ") " << chunk[3]->chunks[k] << "      ";
		else
			std::cout << k << ") " << "                      ";
		if (chunk[6]->chunks[k] != nullptr)
			std::cout << k << ") " << chunk[6]->chunks[k] << "      ";
		else
			std::cout << k << ")" << "                      ";
		std::cout << "\n";
	}

	/*for (int j = 0; j < WORLD_SIZE; j++)
	{
		std::cout << "chunk # " << j << "  ||  memory: " << chunk[j] << "\n";
		for (int k = 0; k < 4; k++)
		{
			if (chunk[j]->chunks[k] != nullptr)
				std::cout << k << ") here is chunk :" << chunk[j]->chunks[k] << "\n";
			else
				std::cout << k << ") here is no chunk \n";
		}
	}*/
}

Chunk* World::getChunk(glm::vec3 position) const
{
	for (int i = 0; i < WORLD_SIZE; i++)
	{
		if (lyingBetween(position.x, 0.0f, chunk[i]->bottom.x - 0.5f, chunk[i]->top.x + 0.5f))
			if (lyingBetween(position.z, 0.0f, chunk[i]->bottom.z - 0.5f, chunk[i]->top.z + 0.5f))
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
	for (int j = 0; j < 2; j++)
	{
		chunk[i]->chunks[j] = getChunk(chunk[i]->bottom - vectors[j]);
	}
	for (int j = 2; j < 4; j++)
	{
		chunk[i]->chunks[j] = getChunk(chunk[i]->top + vectors[j % 2]);
	}
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