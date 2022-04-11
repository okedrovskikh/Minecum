#include "player.h"

Player::Player()
{

}

Player::Player(glm::vec3 position, unsigned int& VAO, unsigned int& VBO, const Shader& shader, const Texture& texture) : camera(glm::vec3(position.x, position.y + HEIGHT_Y / 2 - 0.13f, position.z))
{
	this->position = position;
	this->shader = shader;
	this->texture = texture;

	this->yVelocity = 0.0f;
	this->state = MIDAIR;
	this->interactionBlockIndex = -1;

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

void Player::processMovement(GLFWwindow* window, float deltaTime, const Chunk& chunk)
{
	glm::vec3 motion = glm::vec3(0.0f);

	if (state == STANDING) {
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			state = MIDAIR;
			yVelocity = 8.0f * deltaTime;
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
		yVelocity -= 0.2f * deltaTime;
		break;
	case SWIMING:
		break;
	default:
		break;
	}

	motion = glm::vec3(motion.x, motion.y + yVelocity, motion.z);
	applyMotion(motion, chunk);
}

void Player::rayCast(Chunk& chunk)
{
	float shortestDistance = std::numeric_limits<float>::max();
	interactionBlockIndex = -1;

	for (int i = 0; i < SIZE; i++)
	{
		if (chunk.coordinate[i].second.first == CONTAINER) {

			chunk.coordinate[i].second.second = false;
			
			glm::vec3 solution = camera.Position;
			for (int t = 0; t < INTERACTION_RADIUS * 100; t++)
			{
				solution.x += camera.Front.x * 0.01f;
				bool collisionX = AABB_(solution.x, chunk.coordinate[i].first.x, 0.5f);
				{
					bool collisionY = AABB_(solution.y, chunk.coordinate[i].first.y, 0.5f);
					bool collisionZ = AABB_(solution.z, chunk.coordinate[i].first.z, 0.5f);

					if (collisionX && collisionY && collisionZ) {
						float distance = glm::distance(camera.Position, solution);
						if (distance < shortestDistance) {
							shortestDistance = distance;
							interactionBlockIndex = i;
							break;
						}
					}
				}
				solution.y += camera.Front.y * 0.01f;
				bool collisionY = AABB_(solution.y, chunk.coordinate[i].first.y, 0.5f);
				{
					bool collisionZ = AABB_(solution.z, chunk.coordinate[i].first.z, 0.5f);

					if (collisionX && collisionY && collisionZ) {
						float distance = glm::distance(camera.Position, solution);
						if (distance < shortestDistance) {
							shortestDistance = distance;
							interactionBlockIndex = i;
							break;
						}
					}
				}
				solution.z += camera.Front.z * 0.01f;
				bool collisionZ = AABB_(solution.z, chunk.coordinate[i].first.z, 0.5f);
				{
					if (collisionX && collisionY && collisionZ) {
						float distance = glm::distance(camera.Position, solution);
						if (distance < shortestDistance) {
							shortestDistance = distance;
							interactionBlockIndex = i;
							break;
						}
					}
				}
			}
		}
	}

	if (interactionBlockIndex >= 0)
		chunk.coordinate[interactionBlockIndex].second.second = true;
}

void Player::processLeftClick(Chunk& chunk)
{
	chunk.coordinate[interactionBlockIndex].second.first = AIR;
}

void Player::processRightClick(Chunk& chunk)
{
	updateCamera();

	float shortestDistance = std::numeric_limits<float>::max();
	glm::vec3 newBlock;
	bool done = false;

	if (chunk.coordinate[interactionBlockIndex].second.first == CONTAINER && chunk.coordinate[interactionBlockIndex].second.second == true) {
		glm::vec3 delta[3] = {
			glm::vec3(sgn(camera.Position.x - chunk.coordinate[interactionBlockIndex].first.x), 0.0f, 0.0f),
			glm::vec3(0.0f, sgn(camera.Position.y - chunk.coordinate[interactionBlockIndex].first.y), 0.0f),
			glm::vec3(0.0f, 0.0f, sgn(camera.Position.z - chunk.coordinate[interactionBlockIndex].first.z))
		};

		for (int j = 0; j < 3; j++)
		{
			for (int t = 0; t < INTERACTION_RADIUS * 100; t++)
			{
				float delta_t = t * 0.01f;
				glm::vec3 solution = glm::vec3(camera.Position.x + camera.Front.x * delta_t, camera.Position.y + camera.Front.y * delta_t, camera.Position.z + camera.Front.z * delta_t);

				bool collisionX = AABB_(solution.x, chunk.coordinate[interactionBlockIndex].first.x + delta[j].x, 0.5f);
				bool collisionY = AABB_(solution.y, chunk.coordinate[interactionBlockIndex].first.y + delta[j].y, 0.5f);
				bool collisionZ = AABB_(solution.z, chunk.coordinate[interactionBlockIndex].first.z + delta[j].z, 0.5f);

				if (collisionX && collisionY && collisionZ) {
					float distance = glm::distance(camera.Position, chunk.coordinate[interactionBlockIndex].first + delta[j]);
					if (distance < shortestDistance) {
						shortestDistance = distance;
						newBlock = chunk.coordinate[interactionBlockIndex].first + delta[j];
						done = true;
						break;
					}
				}
			}
		}
	}

	if (done) {
		chunk.coordinate[interactionBlockIndex].second.second = false;
		int index = chunk.getIndex(newBlock);

		bool collisionX = AABB_(newBlock.x - 0.5f, position.x, WIDTH_X / 2) || AABB_(newBlock.x + 0.5f, position.x, WIDTH_X / 2) || AABB_(position.x, newBlock.x, 0.5f);
		bool collisionY = AABB_(newBlock.y - 0.5f, position.y, HEIGHT_Y / 2) || AABB_(newBlock.y + 0.5f, position.y, HEIGHT_Y / 2) || AABB_(position.y, newBlock.y, 0.5f);
		bool collisionZ = AABB_(newBlock.z - 0.5f, position.z, WIDTH_Z / 2) || AABB_(newBlock.z + 0.5f, position.z, WIDTH_Z / 2) || AABB_(position.z, newBlock.z, 0.5f);

		if (!(collisionX && collisionY && collisionZ)) {
			chunk.coordinate[index].second.first = CONTAINER;
			chunk.coordinate[index].second.second = true;
		}
	}
}

void Player::updateCamera()
{
	camera.update(glm::vec3(position.x, position.y + HEIGHT_Y / 2 - 0.13f, position.z));
}

bool Player::AABB_(float one, float two, float parameter)
{
	return (one - (two + parameter)) * (one - (two - parameter)) <= 0;
}

void Player::applyMotion(glm::vec3 motion, const Chunk& chunk)
{
	if (motion.y <= -1.0f)
		motion.y = -1.0f;

	bool overallCollision = false;

	//a little problem with block's angles, cause when i need to fall for some reason i stand in the air
	for (int i = 0; i < SIZE; i++)
	{
		if (chunk.coordinate[i].second.first == CONTAINER) {
			glm::vec3 coord = chunk.coordinate[i].first;

			if (motion.x != 0.0f) {
				glm::vec3 top = glm::vec3(position.x + motion.x + WIDTH_X / 2, position.y + HEIGHT_Y / 2, position.z + WIDTH_Z / 2);
				glm::vec3 bottom = glm::vec3(position.x + motion.x - WIDTH_X / 2, position.y - HEIGHT_Y / 2, position.z - WIDTH_Z / 2);

				bool collisionX = AABB_(top.x, coord.x, 0.5f) || AABB_(bottom.x, coord.x, 0.5f) || AABB_(coord.x, position.x + motion.x, WIDTH_X / 2);
				bool collisionY = AABB_(top.y, coord.y, 0.5f) || AABB_(bottom.y, coord.y, 0.5f) || AABB_(coord.y, position.y + motion.y, HEIGHT_Y / 2);
				bool collisionZ = AABB_(top.z, coord.z, 0.5f) || AABB_(bottom.z, coord.z, 0.5f) || AABB_(coord.z, position.z + motion.z, WIDTH_Z / 2);
				
				if (collisionX && collisionY && collisionZ) 
					motion.x = 0.0f;
			}
			if (motion.y != 0.0f) {
				glm::vec3 top = glm::vec3(position.x + WIDTH_X / 2, position.y + motion.y + HEIGHT_Y / 2, position.z + WIDTH_Z / 2);
				glm::vec3 bottom = glm::vec3(position.x - WIDTH_X / 2, position.y + motion.y - HEIGHT_Y / 2, position.z - WIDTH_Z / 2);

				bool collisionX = AABB_(top.x, coord.x, 0.5f) || AABB_(bottom.x, coord.x, 0.5f) || AABB_(coord.x, position.x + motion.x, WIDTH_X / 2);
				bool collisionY = AABB_(top.y, coord.y, 0.5f) || AABB_(bottom.y, coord.y, 0.5f) || AABB_(coord.y, position.y + motion.y, HEIGHT_Y / 2);
				bool collisionZ = AABB_(top.z, coord.z, 0.5f) || AABB_(bottom.z, coord.z, 0.5f) || AABB_(coord.z, position.z + motion.z, WIDTH_Z / 2);

				if (collisionX && collisionY && collisionZ) {
					if (coord.y + 0.5f <= position.y + motion.y && state != FLYING) 
						state = STANDING;
					else if (position.y + motion.y <= coord.y - 0.5f && state != FLYING) 
						state = MIDAIR;

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

				if (collisionX && collisionY && collisionZ)
					motion.z = 0.0f;
			}
		}
	}

	if (!overallCollision && state != FLYING)
		state = MIDAIR;

	position += motion;
}

Player::~Player()
{

}

int sgn(float a)
{
	if (a > 0.0f)
		return 1;
	else if (a < 0.0f)
		return -1;
	else return 0;
}