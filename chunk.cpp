#include "chunk.h"

Chunk::Chunk()
{

}

Chunk::Chunk(glm::vec3 position)
{
	this->bottom = position;
	this->top = glm::vec3(position.x + CHUNK_SIZE_X, position.y, position.z + CHUNK_SIZE_Z);
	coordinate = new std::pair<glm::vec3, std::pair<BlockType, bool>>[CHUNK_SIZE];

	int countY = -1;
	int countZ = -1;
	for (int countX = 0; countX < CHUNK_SIZE; countX++)
	{
		if (countX % 6 == 0) 
			countZ++;

		if (countZ % 6 == 0 && countX % 6 == 0)
			countY++;
		
		if (countY < 16)
			coordinate[countX] = { glm::vec3(position.x + countX % 6, position.y + countY, position.z + countZ % 6), {CONTAINER, false} };
		else 
			coordinate[countX] = { glm::vec3(position.x + countX % 6, position.y + countY, position.z + countZ % 6), {AIR, false} };
	}
}

int Chunk::getIndex(glm::vec3 position)
{
	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		if (coordinate[i].first.x == position.x && coordinate[i].first.y == position.y && coordinate[i].first.z == position.z)
			return i;
	}

	return -1;
}

Chunk::~Chunk()
{
	delete[] coordinate;
}