#include "entity.h"

Entity::Entity()
{

}

Entity::Entity(glm::vec3 vec, float x, float y, float z, float* vertices, int size) : collider(vec, x, y, z, vertices, size)
{
	position = vec;
	widthX = x;
	height = y;
	widthZ = z;
}

void Entity::updateCollider()
{
	collider.update(position, widthX, height, widthZ);
}

Entity::~Entity()
{

}