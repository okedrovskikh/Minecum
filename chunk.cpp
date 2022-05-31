#include "chunk.h"

Chunk::Chunk()
{
	this->coordinate = new std::pair<glm::vec3, BlockData>;
}

Chunk::Chunk(glm::vec3 position)
{
	this->bottom = position;
	this->top = glm::vec3(position.x + CHUNK_SIZE_X, position.y + CHUNK_SIZE_Y, position.z + CHUNK_SIZE_Z);
	coordinate = new std::pair<glm::vec3, BlockData>[CHUNK_SIZE];

	int countY = -1;
	int countZ = -1;
	for (int countX = 0; countX < CHUNK_SIZE; countX++)
	{
		if (countX % 6 == 0)
			countZ++;

		if (countZ % 6 == 0 && countX % 6 == 0)
			countY++;

		if (countY < 15)
			coordinate[countX] = { glm::vec3(position.x + countX % 6, position.y + countY, position.z + countZ % 6), {STONE, false, true} };
		else if (countY == 15)
			coordinate[countX] = { glm::vec3(position.x + countX % 6, position.y + countY, position.z + countZ % 6), {GRASS, false, true} };
		else
			coordinate[countX] = { glm::vec3(position.x + countX % 6, position.y + countY, position.z + countZ % 6), {AIR, false, false} };
	}
}

int Chunk::getBlockIndex(glm::vec3 position)
{
	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		if (coordinate[i].first.x == position.x && coordinate[i].first.y == position.y && coordinate[i].first.z == position.z)
			return i;
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
				if (coordinate[index].second.type != AIR) {
					int negativeX = getBlockIndex(glm::vec3(x - 1, y, z));
					int positiveX = getBlockIndex(glm::vec3(x + 1, y, z));
					int negativeY = getBlockIndex(glm::vec3(x, y - 1, z));
					int positiveY = getBlockIndex(glm::vec3(x, y + 1, z));
					int negativeZ = getBlockIndex(glm::vec3(x, y, z - 1));
					int positiveZ = getBlockIndex(glm::vec3(x, y, z + 1));
					if (negativeX == -1) {
						if (chunks[0] != nullptr) {
							negativeX = chunks[0]->getBlockIndex(glm::vec3(x - 1, y, z));
							if (chunks[0]->coordinate[negativeX].second.type == AIR) {
								mesh.push_back(coordinate + index);
								continue;
							}
						}
						else {
							mesh.push_back(coordinate + index);
							continue;
						}
					}
					else if (coordinate[negativeX].second.type == AIR) {
						mesh.push_back(coordinate + index);
						continue;
					}
					if (positiveX == -1) {
						if (chunks[2] != nullptr) {
							positiveX = chunks[2]->getBlockIndex(glm::vec3(x + 1, y, z));
							if (chunks[2]->coordinate[positiveX].second.type == AIR) {
								mesh.push_back(coordinate + index);
								continue;
							}
						}
						else {
							mesh.push_back(coordinate + index);
							continue;
						}
					}
					else if (coordinate[positiveX].second.type == AIR) {
						mesh.push_back(coordinate + index);
						continue;
					}
					if (negativeY == -1 || positiveY == -1) {
						mesh.push_back(coordinate + index);
						continue;
					}
					else if (coordinate[negativeY].second.type == AIR || coordinate[positiveY].second.type == AIR) {
						mesh.push_back(coordinate + index);
						continue;
					}
					if (negativeZ == -1) {
						if (chunks[1] != nullptr) {
							negativeZ = chunks[1]->getBlockIndex(glm::vec3(x, y, z - 1));
							if (chunks[1]->coordinate[negativeZ].second.type == AIR) {
								mesh.push_back(coordinate + index);
								continue;
							}
						}
						else {
							mesh.push_back(coordinate + index);
							continue;
						}
					}
					else if (coordinate[negativeZ].second.type == AIR) {
						mesh.push_back(coordinate + index);
						continue;
					}
					if (positiveZ == -1) {
						if (chunks[3] != nullptr) {
							positiveZ = chunks[3]->getBlockIndex(glm::vec3(x, y, z + 1));
							if (chunks[3]->coordinate[positiveZ].second.type == AIR) {
								mesh.push_back(coordinate + index);
								continue;
							}
						}
						else {
							mesh.push_back(coordinate + index);
							continue;
						}
					}
					else if (coordinate[positiveZ].second.type == AIR) {
						mesh.push_back(coordinate + index);
						continue;
					}
				}
			}
		}
	}
}

Chunk::~Chunk()
{
	delete[] coordinate;
}