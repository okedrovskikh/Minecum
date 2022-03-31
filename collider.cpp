#include "collider.h"

Collider::Collider()
{

}

Collider::Collider(glm::vec3 position, float widthX, float height, float widthZ, float* vertices, int size)
{
	top = glm::vec3(position.x + widthX / 2, position.y + height / 2, position.z + widthZ / 2);
	bottom = glm::vec3(position.x - widthX / 2, position.y - height / 2, position.z - widthZ / 2);
	for (int i = 0, count = 0; count < 6; count++)
	{
		glm::vec3 zero = glm::vec3(position.x + vertices[i], position.y + vertices[i + 1], position.z  +vertices[i + 2]); i += 8;
		glm::vec3 one = glm::vec3(position.x + vertices[i], position.y + vertices[i + 1], position.z + vertices[i + 2]); i += 8;
		glm::vec3 two = glm::vec3(position.x + vertices[i], position.y + vertices[i + 1], position.z + vertices[i + 2]); i += 32;
		float a = (one.y - zero.y) * (two.z - zero.z) - (two.y - zero.y) * (one.z - zero.z);
		float b = (one.x - zero.x) * (two.z - zero.z) - (two.x - zero.x) * (one.z - zero.z);
		float c = (one.x - zero.x) * (two.y - zero.y) - (two.x - zero.x) * (one.y - zero.y);
		float d = a * zero.x - b * zero.y + c * zero.z;
		plain[count] = glm::vec4(a, -b, c, -d);
		//normals[count + 1] = -normals[count];
	}
}

void Collider::update(glm::vec3 position, float widthX, float height, float widthZ)
{
	top = glm::vec3(position.x + widthX / 2, position.y + height / 2, position.z + widthZ / 2);
	bottom = glm::vec3(position.x - widthX / 2, position.y - height / 2, position.z - widthZ / 2);
	plainUpdate(position);
}

void Collider::plainUpdate(glm::vec3 position)
{
	for (int i = 0; i < 6; i++)
	{
	
	}
}

bool Collider::AABBCollisionX(Collider other)
{
	if ((bottom.x - other.bottom.x) * (bottom.x - other.top.x) <= 0)
		return true;
	else if ((top.x - other.bottom.x) * (top.x - other.top.x) <= 0)
		return true;
	return false;
}

bool Collider::AABBCollisionY(Collider other)
{
	//std::cout << bottom.y << " " << top.y << " " << other.bottom.y << " " << other.top.y << std::endl;
	if ((bottom.y - other.bottom.y) * (bottom.y - other.top.y) <= 0)
		return true;
	else if ((top.y - other.bottom.y) * (top.y - other.top.y) <= 0)
		return true;
	return false;
}

bool Collider::AABBCollisionZ(Collider other)
{
	if ((bottom.z - other.bottom.z) * (bottom.z - other.top.z) < 0)
		return true;
	else if ((top.z - other.bottom.z) * (top.z - other.top.z) < 0)
		return true;
	return false;
}

glm::vec4 Collider::getClosestPlain(glm::vec3 coordinates)
{
	float minDist = std::numeric_limits<float>::max();
	int numMinDist = NULL;
	for (int i = 0; i < 6; i++)
	{
		float dist = getDistToPlain(i, coordinates);
		if (dist < minDist) {
			minDist = dist;
			numMinDist = i;
		}
	}
	return plain[numMinDist];
}

float Collider::distance(glm::vec3 coordinates)
{
	return fmin(fmin(getDistToPlain(0, coordinates), getDistToPlain(1, coordinates)), fmin(fmin(getDistToPlain(2, coordinates), getDistToPlain(3, coordinates)), fmin(getDistToPlain(4, coordinates), getDistToPlain(5, coordinates))));
}

float Collider::getDistToPlain(int num, glm::vec3 coordinates)
{
	glm::vec4 v = plain[num];
	std::cout << v.x << " " << v.y << " " << v.z << " " << v.w << std::endl;
	return (abs(v.x * coordinates.x + v.y * coordinates.y + v.z * coordinates.z + v.w) / sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

float Collider::getDistToPlain(glm::vec4 plain, glm::vec3 coordinates)
{
	//std::cout << plain.x << " " << plain.y << " " << plain.z << " " << plain.w << std::endl;
	return (abs(plain.x * coordinates.x + plain.y * coordinates.y + plain.z * coordinates.z + plain.w) / sqrt(plain.x * plain.x + plain.y * plain.y + plain.z * plain.z));
}

Collider::~Collider()
{

}
