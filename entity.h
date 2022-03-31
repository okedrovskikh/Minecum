#pragma once
#include "collider.h"

class Entity
{
public:
	glm::vec3 position;
	float widthX;
	float height;
	float widthZ;
	Collider collider;
	Entity();
	Entity(glm::vec3 vec, float x, float y, float z, float* vertices, int size);
	void updateCollider();
	~Entity();
private:
};
