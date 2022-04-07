#include "chunk.h"

Chunk::Chunk(glm::vec3 position)
{
	coordinate = new std::pair<glm::vec3, BlockType>[SIZE];

	int countY = -1;
	int countZ = -1;
	for (int countX = 0; countX < SIZE; countX++)
	{
		if (countX % 6 == 0) 
			countZ++;

		if (countZ % 6 == 0 && countX % 6 == 0)
			countY++;
		
		coordinate[countX] = { glm::vec3(position.x + countX % 6, position.y + countY, position.z + countZ % 6), SOLID };
	}
}


Chunk::~Chunk()
{

}