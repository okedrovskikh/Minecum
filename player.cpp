#include "player.h"

Player::Player()
{

}

Player::Player(glm::vec3 position, unsigned int& VAO, unsigned int& VBO) :
	camera(glm::vec3(position.x, position.y + HEIGHT_Y / 2, position.z))
{
	this->position = position;
	state = MIDAIR;

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

void Player::processMovement(GLFWwindow* window, float deltaTime, Chunk v)
{
	glm::vec3 motion = glm::vec3(0.0f);

	if (state == STANDING) {
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			std::cout << "jump" << std::endl;
			state = MIDAIR;
			yVelocity = 0.18f;
		}
	}
	else if (state == FLYING) {
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			motion += glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			motion -= glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		motion += glm::normalize(glm::vec3(camera.Front.x, 0.0f, camera.Front.z)) * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		motion -= glm::normalize(glm::vec3(camera.Front.x, 0.0f, camera.Front.z)) * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		motion -= glm::normalize(glm::vec3(camera.Right.x, 0.0f, camera.Right.z)) * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		motion += glm::normalize(glm::vec3(camera.Right.x, 0.0f, camera.Right.z)) * deltaTime;

	switch (state)
	{
	case MIDAIR:
		yVelocity -= 0.8f * deltaTime;
		break;
	case SWIMING:
		break;
	default:
		break;
	}

	motion = glm::vec3(motion.x, motion.y + yVelocity, motion.z);
	applyMotion(motion, v);
}

bool Player::AABB(float one, float two)
{
	return (one - (two + 0.5f)) * (one - (two - 0.5f)) <= 0;
}

void Player::applyMotion(glm::vec3 motion, Chunk v)
{
	if (motion.y <= -1.0f)
		motion.y = -1.0f;
	
	glm::vec3 newPosition = position + motion;
	glm::vec3 perpendicularMotion = glm::vec3(-motion.z, 0.0f, motion.x);

	glm::vec3 top = glm::vec3(newPosition.x + 0.3f, newPosition.y + 0.3f, newPosition.z + 0.3f);
	glm::vec3 bottom = glm::vec3(newPosition.x - 0.3f, newPosition.y - 0.3f, newPosition.z - 0.3f);

	bool overallColliding = false;


	for (int i = 0; i < v.coordinate.size(); i++)
	{
		glm::vec3 coord = v.coordinate[i];
		bool collidingX = AABB(top.x, coord.x) || AABB(bottom.x, coord.x);
		bool collidingY = AABB(top.y, coord.y) || AABB(bottom.y, coord.y);
		bool collidingZ = AABB(top.z, coord.z) || AABB(bottom.z, coord.z);
		bool colliding = collidingX && collidingY && collidingZ;
		if (colliding) {
			std::cout << "colliding" << std::endl;
			overallColliding = true;
			if (coord.y + 0.5f < newPosition.y) {
				state = STANDING;
				yVelocity = 0.0f;
				std::cout << "Collision on bottom" << std::endl;
				newPosition.y = round(ceil(position.y * 100) / 10) / 10;
			}
			if ((coord.y - 0.5f <= newPosition.y && newPosition.y <= coord.y + 0.5f)) {
				std::cout << "Coliision on side" << std::endl;
				if (coord.x - 0.5f <= newPosition.x && newPosition.x <= coord.x + 0.5f)
					newPosition.z = round(ceil(position.z * 100) / 10) / 10;
				if (coord.z - 0.5f <= newPosition.z && newPosition.z <= coord.z + 0.5f)
					newPosition.x = round(ceil(position.x * 100) / 10) / 10;
			}
			if (coord.y - 0.5f > newPosition.y) {
				std::cout << "Collision on top" << std::endl;
				newPosition.y = round(ceil(position.y * 100) / 10) / 10;
				state = MIDAIR;
			}
		}
	}

	if (!overallColliding) {
		std::cout << "Not colliding" << std::endl;
		state = MIDAIR;
	}

	position = newPosition;
}

Player::~Player()
{

}