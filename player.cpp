#include "player.h"

Player::Player()
{

}

Player::Player(glm::vec3 position, unsigned int& VAO, unsigned int& VBO) : camera(glm::vec3(position.x, position.y + HEIGHT_Y / 2 - 0.13f, position.z))
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

void Player::processMovement(GLFWwindow* window, float deltaTime, std::vector<glm::vec3> v)
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

void Player::updateCamera()
{
	camera.update(glm::vec3(position.x, position.y + HEIGHT_Y / 2 - 0.13f, position.z));
}

bool Player::AABB(float one, float two)
{
	return (one - (two + 0.5f)) * (one - (two - 0.5f)) <= 0;
}

bool Player::AABB_(float one, float two, float par)
{
	return (one - (two + par)) * (one - (two - par)) <= 0;
}

void Player::applyMotion(glm::vec3 motion, std::vector<glm::vec3> v)
{
	if (motion.y <= -1.0f)
		motion.y = -1.0f;

	bool overallCollision = false;

	int size = v.size();
    //a little problem with angle, cause i need to fall, but for some reason i stand in the air
	for (int i = 0; i < size; i++)
	{
		glm::vec3 coord = v[i];

		if (motion.x != 0.0f) {
			glm::vec3 top = glm::vec3(position.x + motion.x + WIDTH_X / 2, position.y + HEIGHT_Y / 2, position.z + WIDTH_Z / 2);
			glm::vec3 bottom = glm::vec3(position.x + motion.x - WIDTH_X / 2, position.y - HEIGHT_Y / 2, position.z - WIDTH_Z / 2);

			bool collisionX = AABB(top.x, coord.x) || AABB(bottom.x, coord.x) || AABB_(coord.x, position.x + motion.x, WIDTH_X / 2);
			bool collisionY = AABB(top.y, coord.y) || AABB(bottom.y, coord.y) || AABB_(coord.y, position.y + motion.y, HEIGHT_Y / 2);
			bool collisionZ = AABB(top.z, coord.z) || AABB(bottom.z, coord.z) || AABB_(coord.z, position.z + motion.z, WIDTH_Z / 2);
			bool collision = collisionX && collisionY && collisionZ;
			if (collision) {
				std::cout << "Collision on X" << std::endl;
				motion.x = 0.0f;
			}
		}
		if (motion.y != 0.0f) {
			glm::vec3 top = glm::vec3(position.x + WIDTH_X / 2, position.y + motion.y + HEIGHT_Y / 2, position.z + WIDTH_Z / 2);
			glm::vec3 bottom = glm::vec3(position.x - WIDTH_X / 2, position.y + motion.y - HEIGHT_Y / 2, position.z - WIDTH_Z / 2);

			bool collisionX = AABB(top.x, coord.x) || AABB(bottom.x, coord.x) || AABB_(coord.x, position.x + motion.x, WIDTH_X / 2);
			bool collisionY = AABB(top.y, coord.y) || AABB(bottom.y, coord.y) || AABB_(coord.y, position.y + motion.y, HEIGHT_Y / 2);
			bool collisionZ = AABB(top.z, coord.z) || AABB(bottom.z, coord.z) || AABB_(coord.z, position.z + motion.z, WIDTH_Z / 2);
			bool collision = collisionX && collisionY && collisionZ;
			if (collision) {
				if (coord.y + 0.5f <= position.y + motion.y) {
					std::cout << "Collision on bottom Y " << coord.x << " " << coord.y << " " << coord.z << std::endl;
					state = STANDING;
				}
				else if (position.y + motion.y <= coord.y - 0.5f) {
					std::cout << "Collision on top Y" << std::endl;
					state = MIDAIR;
				}
				motion.y = 0.0f;
				yVelocity = 0.0f;
				overallCollision = true;
			}
		}
		if (motion.z != 0.0f) {
			glm::vec3 top = glm::vec3(position.x + WIDTH_X / 2, position.y + HEIGHT_Y / 2, position.z + motion.z + WIDTH_Z / 2);
			glm::vec3 bottom = glm::vec3(position.x - WIDTH_X / 2, position.y - HEIGHT_Y / 2, position.z + motion.z - WIDTH_Z / 2);

			bool collisionX = AABB(top.x, coord.x) || AABB(bottom.x, coord.x) || AABB_(coord.x, position.x + motion.x, WIDTH_X / 2);
			bool collisionY = AABB(top.y, coord.y) || AABB(bottom.y, coord.y) || AABB_(coord.y, position.y + motion.y, HEIGHT_Y / 2);
			bool collisionZ = AABB(top.z, coord.z) || AABB(bottom.z, coord.z) || AABB_(coord.z, position.z + motion.z, WIDTH_Z / 2);
			bool collision = collisionX && collisionY && collisionZ;
			if (collision) {
				std::cout << "Collision on Z" << std::endl;
				motion.z = 0.0f;
			}
		}
	}

	if (!overallCollision) {
		state = MIDAIR;
	}

	position += motion;
}

Player::~Player()
{

}

float distance(glm::vec3 p1, glm::vec3 p2)
{
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.x - p2.z));
}