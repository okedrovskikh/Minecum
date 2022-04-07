#include "player.h"

Player::Player()
{

}

Player::Player(glm::vec3 position, unsigned int& VAO, unsigned int& VBO, Shader& shader, Texture& texture) : camera(glm::vec3(position.x, position.y + HEIGHT_Y / 2 - 0.13f, position.z))
{
	this->position = position;
	this->shader = shader;
	this->texture = texture;

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

void Player::processMovement(GLFWwindow* window, float deltaTime, const Chunk& chunk)
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
	applyMotion(motion, chunk);
}

void Player::processLeftClick(Chunk& chunk)
{
	float shortest = std::numeric_limits<float>::max();
	int result = NULL;
	bool done = false;

	updateCamera();

	for (int i = 0; i < SIZE; i++)
	{
		if (chunk.coordinate[i].second == SOLID) {
			glm::vec3 coord = chunk.coordinate[i].first;

			for (int t = 0; t < 10; t++) {
				float delta_t = (float)t / 10;

				bool collisionX = AABB_(camera.Position.x + camera.Front.x * delta_t, coord.x, 0.5f);
				bool collisionY = AABB_(camera.Position.y + camera.Front.y * delta_t, coord.y, 0.5f);
				bool collisionZ = AABB_(camera.Position.z + camera.Front.z * delta_t, coord.z, 0.5f);

				if (collisionX && collisionY && collisionZ) {
					float dist = glm::distance(coord, camera.Position);
					if (dist < shortest) {
						shortest = dist;
						result = i;
						done = true;
					}
					break;
				}
			}
		}
	}

	if (done) {
		chunk.coordinate[result].second = AIR;
		std::cout << chunk.coordinate[result].first.x << " " << chunk.coordinate[result].first.y << " " << chunk.coordinate[result].first.z << std::endl;
	}
}

void Player::processRightClick(Chunk& chunk)
{
	updateCamera();

	bool done = false;
	int result = NULL;

	for (int i = 0; i < SIZE; i++)
	{
		if (chunk.coordinate[i].second == AIR) {
			glm::vec3 coord = chunk.coordinate[i].first;

			for (int t = 0; t < 10; t++) {
				float delta_t = (float)t / 10;

				bool collisionX = AABB_(camera.Position.x + camera.Front.x * delta_t, coord.x, 0.5f);
				bool collisionY = AABB_(camera.Position.y + camera.Front.y * delta_t, coord.y, 0.5f);
				bool collisionZ = AABB_(camera.Position.z + camera.Front.z * delta_t, coord.z, 0.5f);

				if (collisionX && collisionY && collisionZ) {
					result = i;
					done = true;
					break;
				}
			}

			if (done)
				break;
		}
	}

	if (done) {
		chunk.coordinate[result].second = SOLID;
		std::cout << chunk.coordinate[result].first.x << " " << chunk.coordinate[result].first.y << " " << chunk.coordinate[result].first.z << std::endl;
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
		if (chunk.coordinate[i].second == SOLID) {
			glm::vec3 coord = chunk.coordinate[i].first;

			if (motion.x != 0.0f) {
				glm::vec3 top = glm::vec3(position.x + motion.x + WIDTH_X / 2, position.y + HEIGHT_Y / 2, position.z + WIDTH_Z / 2);
				glm::vec3 bottom = glm::vec3(position.x + motion.x - WIDTH_X / 2, position.y - HEIGHT_Y / 2, position.z - WIDTH_Z / 2);

				bool collisionX = AABB_(top.x, coord.x, 0.5f) || AABB_(bottom.x, coord.x, 0.5f) || AABB_(coord.x, position.x + motion.x, WIDTH_X / 2);
				bool collisionY = AABB_(top.y, coord.y, 0.5f) || AABB_(bottom.y, coord.y, 0.5f) || AABB_(coord.y, position.y + motion.y, HEIGHT_Y / 2);
				bool collisionZ = AABB_(top.z, coord.z, 0.5f) || AABB_(bottom.z, coord.z, 0.5f) || AABB_(coord.z, position.z + motion.z, WIDTH_Z / 2);
				if (collisionX && collisionY && collisionZ) {
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
						state = STANDING;
					}
					else if (position.y + motion.y <= coord.y - 0.5f) {
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
					motion.z = 0.0f;
				}
			}
		}
	}

	if (!overallCollision && state != FLYING) {
		state = MIDAIR;
	}

	position += motion;
}

Player::~Player()
{

}

int sgn(float a)
{
	if (a > 0)
		return 1;
	else if (a < 0)
		return -1;
	else return 0;
}