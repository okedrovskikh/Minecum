#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Collider
{
public:
	glm::vec4 plain[6];
	glm::vec3 top;
	glm::vec3 bottom;
	Collider();
	Collider(glm::vec3 position, float widthX, float height, float widthZ, float* vertices, int size);
	void update(glm::vec3 position, float widthX, float height, float widthZ);
	void plainUpdate(glm::vec3 position);
	glm::vec4 getClosestPlain(glm::vec3 coordinates);
	bool AABBCollisionX(Collider other);
	bool AABBCollisionY(Collider other);
	bool AABBCollisionZ(Collider other);
	float distance(glm::vec3 coordinates);
	float getDistToPlain(int num, glm::vec3 coordinates);
	float getDistToPlain(glm::vec4 plain, glm::vec3 coordinates);
	~Collider();
private:
};
