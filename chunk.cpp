#include "chunk.h"

Chunk::Chunk()
{
	this->coordinate = new BlockData[CHUNK_SIZE];
}

Chunk::Chunk(glm::vec3 position)
{
	this->bottom = position;
	this->top = glm::vec3(position.x + CHUNK_SIZE_X - 1, position.y + CHUNK_SIZE_Y, position.z + CHUNK_SIZE_Z - 1);
	this->coordinate = new BlockData[CHUNK_SIZE];

	std::array<int, CHUNK_SIZE_X* CHUNK_SIZE_Z> heightMap = createChunkHeightMap(position);

	for (int z = 0; z < CHUNK_SIZE_Z; z++)
	{
		for (int x = 0; x < CHUNK_SIZE_X; x++)
		{
			int height = heightMap[z * CHUNK_SIZE_X + x];
			for (int y = 0; y < CHUNK_SIZE_Y; y++)
			{
				if (y <= height - 3) {
					coordinate[(z * CHUNK_SIZE_Z + x) * CHUNK_SIZE_Y + y] = { glm::vec3(position.x + x, position.y + y, position.z + z), STONE, false, true };
				}
				else if (y <= height) {
					coordinate[(z * CHUNK_SIZE_Z + x) * CHUNK_SIZE_Y + y] = { glm::vec3(position.x + x, position.y + y, position.z + z), GRASS, false, true };
				}
				else {
					coordinate[(z * CHUNK_SIZE_Z + x) * CHUNK_SIZE_Y + y] = { glm::vec3(position.x + x, position.y + y ,position.z + z), AIR, false, false };
				}
			}
		}
	}
}

int Chunk::getBlockIndex(glm::vec3 position)
{
	position = glm::vec3(round(position.x), round(position.y), round(position.z));

	for (int z = 0; z < CHUNK_SIZE_Z; z++)
	{
		if (z + bottom.z == position.z) {
			for (int x = 0; x < CHUNK_SIZE_X; x++)
			{
				if (x + bottom.x == position.x) {
					for (int y = 0; y < CHUNK_SIZE_Y; y++)
					{
						if (y + bottom.y == position.y) {
							return (z * CHUNK_SIZE_X + x) * CHUNK_SIZE_Y + y;
						}
					}
				}
			}
		}
	}

	return -1;
}

void Chunk::updateMesh()
{
	mesh.clear();
	for (float y = bottom.y; y <= top.y; y++)
	{
		for (float z = bottom.z; z <= top.z; z++)
		{
			for (float x = bottom.x; x <= top.x; x++)
			{
				int index = getBlockIndex(glm::vec3(x, y, z));
				if (coordinate[index].type != AIR) {
					int negativeX = getBlockIndex(glm::vec3(x - 1, y, z));
					if (negativeX == -1) {
						if (chunks[0] != nullptr) {
							negativeX = chunks[0]->getBlockIndex(glm::vec3(x - 1, y, z));
							if (chunks[0]->coordinate[negativeX].type == AIR) {
								mesh.push_back(coordinate + index);
								continue;
							}
						}
						else {
							mesh.push_back(coordinate + index);
							continue;
						}
					}
					else if (coordinate[negativeX].type == AIR) {
						mesh.push_back(coordinate + index);
						continue;
					}
					int positiveX = getBlockIndex(glm::vec3(x + 1, y, z));
					if (positiveX == -1) {
						if (chunks[2] != nullptr) {
							positiveX = chunks[2]->getBlockIndex(glm::vec3(x + 1, y, z));
							if (chunks[2]->coordinate[positiveX].type == AIR) {
								mesh.push_back(coordinate + index);
								continue;
							}
						}
						else {
							mesh.push_back(coordinate + index);
							continue;
						}
					}
					else if (coordinate[positiveX].type == AIR) {
						mesh.push_back(coordinate + index);
						continue;
					}
					int negativeY = getBlockIndex(glm::vec3(x, y - 1, z));
					int positiveY = getBlockIndex(glm::vec3(x, y + 1, z));
					if (negativeY == -1 || positiveY == -1) {
						mesh.push_back(coordinate + index);
						continue;
					}
					else if (coordinate[negativeY].type == AIR || coordinate[positiveY].type == AIR) {
						mesh.push_back(coordinate + index);
						continue;
					}
					int negativeZ = getBlockIndex(glm::vec3(x, y, z - 1));
					if (negativeZ == -1) {
						if (chunks[1] != nullptr) {
							negativeZ = chunks[1]->getBlockIndex(glm::vec3(x, y, z - 1));
							if (chunks[1]->coordinate[negativeZ].type == AIR) {
								mesh.push_back(coordinate + index);
								continue;
							}
						}
						else {
							mesh.push_back(coordinate + index);
							continue;
						}
					}
					else if (coordinate[negativeZ].type == AIR) {
						mesh.push_back(coordinate + index);
						continue;
					}
					int positiveZ = getBlockIndex(glm::vec3(x, y, z + 1));
					if (positiveZ == -1) {
						if (chunks[3] != nullptr) {
							positiveZ = chunks[3]->getBlockIndex(glm::vec3(x, y, z + 1));
							if (chunks[3]->coordinate[positiveZ].type == AIR) {
								mesh.push_back(coordinate + index);
								continue;
							}
						}
						else {
							mesh.push_back(coordinate + index);
							continue;
						}
					}
					else if (coordinate[positiveZ].type == AIR) {
						mesh.push_back(coordinate + index);
						continue;
					}
				}
			}
		}
	}
}

glm::vec3 Chunk::getMaxHeight(glm::vec3 position)
{
	glm::vec3 result = position;
	int index = -1;

	for (int y = bottom.y; y < top.y; y++)
	{
		index = getBlockIndex(glm::vec3(result.x, y, result.z));
		if (coordinate[index].type != AIR)
			result = coordinate[index].coord + glm::vec3(0.0f, 1.0f, 0.0f);
	}

	return result;
}

std::array<int, CHUNK_SIZE_X* CHUNK_SIZE_Z> Chunk::createChunkHeightMap(glm::vec3 position)
{
	std::array<int, CHUNK_SIZE_X* CHUNK_SIZE_Z> heightMap;

	for (int z = 0; z < CHUNK_SIZE_Z; z++)
	{
		for (int x = 0; x < CHUNK_SIZE_X; x++)
		{
			float value = glm::simplex(glm::vec2((position.x + x) / 64.0f, (position.z + z) / 64.0f));
			value = (value + 1.0f) / 2;
			value *= CHUNK_SIZE_Y / 2;
			heightMap[z * CHUNK_SIZE_X + x] = static_cast<int>(value) + CHUNK_SIZE_Y / 2;
		}
	}

	return heightMap;
}

Chunk::~Chunk()
{
	delete[] coordinate;
}