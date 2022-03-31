#include "player.h"

Player::Player()
{

}

Player::Player(glm::vec3 position, unsigned int& VAO, unsigned int& VBO) :
	fly(false), grounded(false), entity(position, WIDTH_X, HEIGHT_Y, WIDTH_Z, vertices, sizeof(vertices) / sizeof(float)), camera(glm::vec3(entity.position.x, entity.position.y + entity.height / 2, entity.position.z))
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Player::ProcessKeyboard(Movement direction, float deltaTime)
{
	float velocity = 2.0f * deltaTime;
	if (direction == FORWARD){
		entity.position += camera.Front * velocity;
	}
	else if (direction == BACKWARD){
		entity.position -= camera.Front * velocity;
	}
	else if (direction == RIGHT){
		//Это для финальной версии
		//Position += glm::cross(directionVec, glm::vec3(0.0f, 1.0f, 0.0f)) * velocity;
		entity.position += camera.Right * velocity;
	}
	else if (direction == LEFT){
		//Это для финальной версии
		//Position += glm::cross(directionVec, glm::vec3(0.0f, 1.0f, 0.0f)) * velocity;
		entity.position -= camera.Right * velocity;
	}
	else if (direction == UP) {
		if (grounded) {
			//std::cout << "jump" << std::endl;
			entity.position.y += 500.0f * deltaTime;
		}
		grounded = false;
	}
}

void Player::checkCollisionAABB(bool& collisionX, bool& collisionY, bool& collisionZ, Collider other)
{
	collisionX = entity.collider.AABBCollisionX(other);
	collisionY = entity.collider.AABBCollisionY(other);
	collisionZ = entity.collider.AABBCollisionZ(other);
}

void Player::fall(bool fall, float deltaTime)
{
	if (fall && !fly) {
		entity.position.y -= 1.0f * deltaTime;
		grounded = false;
	}
	else if (fly) {

	}
	else {
		entity.position.y = round(entity.position.y * 10) / 10;
		grounded = true;
	}
}

void Player::setFly(bool set)
{
	fly = set;
}

Player::~Player()
{

}