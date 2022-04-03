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

void Player::processMovement(GLFWwindow* window, float deltaTime, const std::vector<glm::vec3>& v)
{
	glm::vec3 motion = glm::vec3(0.0f);

	if (state == STANDING) {
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			state = MIDAIR;
			yVelocity = 0.12f;
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

	if (motion.x != 0.0f || motion.y != 0.0f || motion.z != 0.0f)
		applyMotion(motion, v);
}

void Player::updateCamera()
{
	camera.update(glm::vec3(position.x, position.y + HEIGHT_Y / 2 - 0.13f, position.z));
}

bool Player::AABB_(float one, float two, float par)
{
	return (one - (two + par)) * (one - (two - par)) <= 0;
}

void Player::applyMotion(glm::vec3 motion, const std::vector<glm::vec3>& v)
{
	if (motion.y <= -1.0f)
		motion.y = -1.0f;

	bool overallCollision = false;

	int size = v.size();
    //a little problem with block's angles, cause when i need to fall for some reason i stand in the air
	for (int i = 0; i < size; i++)
	{
		glm::vec3 coord = v[i];

		if (motion.x != 0.0f) {
			glm::vec3 top = glm::vec3(position.x + motion.x + WIDTH_X / 2, position.y + HEIGHT_Y / 2, position.z + WIDTH_Z / 2);
			glm::vec3 bottom = glm::vec3(position.x + motion.x - WIDTH_X / 2, position.y - HEIGHT_Y / 2, position.z - WIDTH_Z / 2);

			bool collisionX = AABB_(top.x, coord.x, 0.5f) || AABB_(bottom.x, coord.x, 0.5f) || AABB_(coord.x, position.x + motion.x, WIDTH_X / 2);
			bool collisionY = AABB_(top.y, coord.y, 0.5f) || AABB_(bottom.y, coord.y, 0.5f) || AABB_(coord.y, position.y + motion.y, HEIGHT_Y / 2);
			bool collisionZ = AABB_(top.z, coord.z, 0.5f) || AABB_(bottom.z, coord.z, 0.5f) || AABB_(coord.z, position.z + motion.z, WIDTH_Z / 2);
			if (collisionX && collisionY && collisionZ) {
				//std::cout << "Collision on X" << std::endl;
				motion.x = 0.0f;
			}
		}
		if (motion.y != 0.0f) {
			glm::vec3 top = glm::vec3(position.x + WIDTH_X / 2, position.y + motion.y + HEIGHT_Y / 2, position.z + WIDTH_Z / 2);
			glm::vec3 bottom = glm::vec3(position.x - WIDTH_X / 2, position.y + motion.y - HEIGHT_Y / 2, position.z - WIDTH_Z / 2);

			bool collisionX = AABB_(top.x, coord.x, 0.5f) || AABB_(bottom.x, coord.x, 0.5f) || AABB_(coord.x, position.x + motion.x, WIDTH_X / 2);
			bool collisionY = AABB_(top.y, coord.y, 0.5f) || AABB_(bottom.y, coord.y, 0.5f) || AABB_(coord.y, position.y + motion.y, HEIGHT_Y / 2);
			bool collisionZ = AABB_(top.z, coord.z, 0.5f) || AABB_(bottom.z, coord.z, 0.5f) || AABB_(coord.z, position.z + motion.z, WIDTH_Z / 2);
			if (collisionX && collisionY && collisionZ) {
				if (coord.y + 0.5f <= position.y + motion.y) {
					//std::cout << "Collision on bottom Y " << coord.x << " " << coord.y << " " << coord.z << std::endl;
					state = STANDING;
				}
				else if (position.y + motion.y <= coord.y - 0.5f) {
					//std::cout << "Collision on top Y" << std::endl;
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

			bool collisionX = AABB_(top.x, coord.x, 0.5f) || AABB_(bottom.x, coord.x, 0.5f) || AABB_(coord.x, position.x + motion.x, WIDTH_X / 2);
			bool collisionY = AABB_(top.y, coord.y, 0.5f) || AABB_(bottom.y, coord.y, 0.5f) || AABB_(coord.y, position.y + motion.y, HEIGHT_Y / 2);
			bool collisionZ = AABB_(top.z, coord.z, 0.5f) || AABB_(bottom.z, coord.z, 0.5f) || AABB_(coord.z, position.z + motion.z, WIDTH_Z / 2);
			if (collisionX && collisionY && collisionZ) {
				//std::cout << "Collision on Z" << std::endl;
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