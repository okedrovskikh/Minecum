#include "chunk.h"

Chunk::Chunk()
{

}

bool Chunk::isInChunk(glm::vec3 p)
{
	for (int i = 0; i < coordinate.size(); i++)
	{
		if (p.x == coordinate[i].x)
			if (p.y == coordinate[i].y)
				if (p.z == coordinate[i].z) {
					std::cout << coordinate[i].x << " " << coordinate[i].y << " " << coordinate[i].z << std::endl;
					return true;
				}
	}
	return false;
}

bool Chunk::isInChunk(float x, float y, float z)
{
	return isInChunk(glm::vec3(x, y, z));
}

bool Chunk::isInChunk(int x, int y, int z)
{
	return isInChunk(glm::vec3(x, y, z));
}

Chunk::~Chunk()
{

}