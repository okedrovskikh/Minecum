#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <vector>

const unsigned int SIZE = 6 * 6 * 16;

class Chunk
{
public:
	std::vector<glm::vec3> coordinate;
	Chunk(glm::vec3 position);
	~Chunk();
private:
};
