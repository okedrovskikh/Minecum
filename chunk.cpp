#include "chunk.h"

Chunk::Chunk()
{

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
	updateMesh();
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
				//if (index != -1) {
					if (coordinate[index].second.type != AIR) {
						if (coordinate[getBlockIndex(glm::vec3(x - 1, y, z))].second.type == AIR || coordinate[getBlockIndex(glm::vec3(x, y - 1, z))].second.type == AIR || coordinate[getBlockIndex(glm::vec3(x, y, z - 1))].second.type == AIR ||
							coordinate[getBlockIndex(glm::vec3(x + 1, y, z))].second.type == AIR || coordinate[getBlockIndex(glm::vec3(x, y + 1, z))].second.type == AIR || coordinate[getBlockIndex(glm::vec3(x, y, z + 1))].second.type == AIR) {
							mesh.push_back(coordinate[index]);
						}
					}
				//}
			}
		}
	}
}

Chunk::~Chunk()
{
	delete[] coordinate;
}