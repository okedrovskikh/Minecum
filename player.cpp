#include "player.h"

Player::Player()
{
	this->shader = nullptr;
	this->texture = nullptr;
}

Player::Player(glm::vec3 position, std::string vertexPath, std::string fragmentPath, std::string texturePath) : camera(glm::vec3(position.x, position.y + PLAYER_SIZE_Y / 2 - 0.13f, position.z))
{
	this->position = position;
	this->shader = new Shader(vertexPath.c_str(), fragmentPath.c_str());
	this->texture = new Texture(texturePath);
	this->chosenBlock = GRASS;

	this->yVelocity = 0.0f;
	this->state = FLYING;
	this->interactionBlockIndex = -1;
	this->interactionChunkIndex = -1;

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

void Player::processMovement(GLFWwindow* window, float deltaTime, const std::vector<Chunk*>& chunks)
{
	glm::vec3 motion = glm::vec3(0.0f);

	if (state == STANDING) {
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			state = MIDAIR;
			yVelocity = 0.12f;
		}
	}
	else if (state == FLYING) {
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			motion += glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			motion -= glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		motion += MOVEMENT_SPEED * glm::normalize(glm::vec3(camera.Front.x, 0.0f, camera.Front.z)) * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		motion -= MOVEMENT_SPEED * glm::normalize(glm::vec3(camera.Front.x, 0.0f, camera.Front.z)) * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		motion -= MOVEMENT_SPEED * glm::normalize(glm::vec3(camera.Right.x, 0.0f, camera.Right.z)) * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		motion += MOVEMENT_SPEED * glm::normalize(glm::vec3(camera.Right.x, 0.0f, camera.Right.z)) * deltaTime;

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
	applyMotion(motion, chunks);
}

void Player::rayCast(std::vector<Chunk*>& chunks)
{
	float shortestDistance = fMax;
	interactionChunkIndex = -1;
	interactionBlockIndex = -1;

	int* order = getOrder();
	for (int l = 0; l < chunks.size(); l++)
	{
		if (chunks[l] != nullptr) {
			for (int i = 0; i < CHUNK_SIZE; i++)
			{
				if (chunks[l]->coordinate[i].type != AIR) {
					chunks[l]->coordinate[i].chosen = false;

					glm::vec3 coord = chunks[l]->coordinate[i].coord;
					glm::vec3 solution = camera.Position;

					bool collisionX = axisCollision(solution.x, coord.x, 0.5f);
					bool collisionY = axisCollision(solution.y, coord.y, 0.5f);
					bool collisionZ = axisCollision(solution.z, coord.z, 0.5f);


					for (int t = 0; t < INTERACTION_RADIUS * 50; t++)
					{
						for (int j = 0; j < 3; j++)
						{
							switch (order[j])
							{
							case(1):
								solution.x += camera.Front.x * 0.02f;
								collisionX = axisCollision(solution.x, coord.x, 0.5f);
								if (collisionX && collisionY && collisionZ) {
									float distance = glm::distance(camera.Position, solution);
									if (distance < shortestDistance) {
										shortestDistance = distance;
										interactionChunkIndex = l;
										interactionBlockIndex = i;
									}
								}
								break;
							case(2):
								solution.y += camera.Front.y * 0.02f;
								collisionY = axisCollision(solution.y, coord.y, 0.5f);
								if (collisionX && collisionY && collisionZ) {
									float distance = glm::distance(camera.Position, solution);
									if (distance < shortestDistance) {
										shortestDistance = distance;
										interactionChunkIndex = l;
										interactionBlockIndex = i;
									}
								}
								break;
							case(3):
								solution.z += camera.Front.z * 0.02f;
								collisionZ = axisCollision(solution.z, coord.z, 0.5f);
								if (collisionX && collisionY && collisionZ) {
									float distance = glm::distance(camera.Position, solution);
									if (distance < shortestDistance) {
										shortestDistance = distance;
										interactionChunkIndex = l;
										interactionBlockIndex = i;
									}
								}
								break;
							default:
								break;
							}
						}
					}
				}
			}
		}
	}
	delete[] order;

	if (interactionChunkIndex != -1 && interactionBlockIndex != -1)
		chunks[interactionChunkIndex]->coordinate[interactionBlockIndex].chosen = true;
}

void Player::processLeftClick(std::vector<Chunk*>& chunks)
{
	if (interactionChunkIndex != -1 && interactionBlockIndex != -1) {
		chunks[interactionChunkIndex]->coordinate[interactionBlockIndex].type = AIR;
		chunks[interactionChunkIndex]->updateMesh();
		if (chunks[interactionChunkIndex]->coordinate[interactionBlockIndex].coord.x == chunks[interactionChunkIndex]->bottom.x && chunks[interactionChunkIndex]->chunks[0] != nullptr)
			chunks[interactionChunkIndex]->chunks[0]->updateMesh();
		if (chunks[interactionChunkIndex]->coordinate[interactionBlockIndex].coord.z == chunks[interactionChunkIndex]->bottom.z && chunks[interactionChunkIndex]->chunks[1] != nullptr)
			chunks[interactionChunkIndex]->chunks[1]->updateMesh();
		if (chunks[interactionChunkIndex]->coordinate[interactionBlockIndex].coord.x == chunks[interactionChunkIndex]->top.x && chunks[interactionChunkIndex]->chunks[2] != nullptr)
			chunks[interactionChunkIndex]->chunks[2]->updateMesh();
		if (chunks[interactionChunkIndex]->coordinate[interactionBlockIndex].coord.z == chunks[interactionChunkIndex]->top.z && chunks[interactionChunkIndex]->chunks[3] != nullptr)
			chunks[interactionChunkIndex]->chunks[3]->updateMesh();
	}
}

void Player::processRightClick(std::vector<Chunk*>& chunks)
{
	if (interactionChunkIndex == -1 || interactionBlockIndex == -1)
		return;
	
	float shortestDistance = fMax;
	glm::vec3 newBlock;
	bool done = false;

	if (chunks[interactionChunkIndex]->coordinate[interactionBlockIndex].chosen == true) {
		glm::vec3 delta[3] = {
			glm::vec3(sgn(camera.Position.x - chunks[interactionChunkIndex]->coordinate[interactionBlockIndex].coord.x), 0.0f, 0.0f),
			glm::vec3(0.0f, sgn(camera.Position.y - chunks[interactionChunkIndex]->coordinate[interactionBlockIndex].coord.y), 0.0f),
			glm::vec3(0.0f, 0.0f, sgn(camera.Position.z - chunks[interactionChunkIndex]->coordinate[interactionBlockIndex].coord.z))
		};

		int* order = getOrder();
		for (int i = 0; i < 3; i++)
		{
			glm::vec3 newCoord = chunks[interactionChunkIndex]->coordinate[interactionBlockIndex].coord + delta[i];
			glm::vec3 solution = camera.Position;

			bool collisionX = axisCollision(solution.x, newCoord.x, 0.5f);
			bool collisionY = axisCollision(solution.y, newCoord.y, 0.5f);
			bool collisionZ = axisCollision(solution.z, newCoord.z, 0.5f);


			for (int t = 0; t < INTERACTION_RADIUS * 50; t++)
			{
				for (int j = 0; j < 3; j++)
				{
					switch (order[j])
					{
					case(1):
						solution.x += camera.Front.x * 0.02f;
						collisionX = axisCollision(solution.x, newCoord.x, 0.5f);
						if (collisionX && collisionY && collisionZ) {
							float distance = glm::distance(camera.Position, newCoord);
							if (distance < shortestDistance) {
								shortestDistance = distance;
								newBlock = newCoord;
								done = true;
							}
						}
						break;
					case(2):
						solution.y += camera.Front.y * 0.02f;
						collisionY = axisCollision(solution.y, newCoord.y, 0.5f);
						if (collisionX && collisionY && collisionZ) {
							float distance = glm::distance(camera.Position, newCoord);
							if (distance < shortestDistance) {
								shortestDistance = distance;
								newBlock = newCoord;
								done = true;
							}
						}
						break;
					case(3):
						solution.z += camera.Front.z * 0.02f;
						collisionZ = axisCollision(solution.z, newCoord.z, 0.5f);
						if (collisionX && collisionY && collisionZ) {
							float distance = glm::distance(camera.Position, newCoord);
							if (distance < shortestDistance) {
								shortestDistance = distance;
								newBlock = newCoord;
								done = true;
							}
						}
						break;
					default:
						break;
					}
				}
			}
		}
		delete[] order;
	}

	if (done) {
		for (int i = 0; i < chunks.size(); i++) {
			int index = chunks[i]->getBlockIndex(newBlock);
			if (index != -1) {
				bool collisionX = axisCollision(newBlock.x - 0.5f, position.x, PLAYER_SIZE_X / 2) || axisCollision(newBlock.x + 0.5f, position.x, PLAYER_SIZE_X / 2) || axisCollision(position.x, newBlock.x, 0.5f);
				bool collisionY = axisCollision(newBlock.y - 0.5f, position.y, PLAYER_SIZE_Y / 2) || axisCollision(newBlock.y + 0.5f, position.y, PLAYER_SIZE_Y / 2) || axisCollision(position.y, newBlock.y, 0.5f);
				bool collisionZ = axisCollision(newBlock.z - 0.5f, position.z, PLAYER_SIZE_Z / 2) || axisCollision(newBlock.z + 0.5f, position.z, PLAYER_SIZE_Z / 2) || axisCollision(position.z, newBlock.z, 0.5f);

				if (!(collisionX && collisionY && collisionZ)) {
					chunks[interactionChunkIndex]->coordinate[interactionBlockIndex].chosen = false;
					chunks[i]->coordinate[index].type = chosenBlock;
					chunks[i]->coordinate[index].chosen = true;
					chunks[i]->updateMesh();
					if (chunks[interactionChunkIndex]->coordinate[interactionBlockIndex].coord.x == chunks[interactionChunkIndex]->bottom.x && chunks[interactionChunkIndex]->chunks[0] != nullptr)
						chunks[interactionChunkIndex]->chunks[0]->updateMesh();
					if (chunks[interactionChunkIndex]->coordinate[interactionBlockIndex].coord.z == chunks[interactionChunkIndex]->bottom.z && chunks[interactionChunkIndex]->chunks[1] != nullptr)
						chunks[interactionChunkIndex]->chunks[1]->updateMesh();
					if (chunks[interactionChunkIndex]->coordinate[interactionBlockIndex].coord.x == chunks[interactionChunkIndex]->top.x && chunks[interactionChunkIndex]->chunks[2] != nullptr)
						chunks[interactionChunkIndex]->chunks[2]->updateMesh();
					if (chunks[interactionChunkIndex]->coordinate[interactionBlockIndex].coord.z == chunks[interactionChunkIndex]->top.z && chunks[interactionChunkIndex]->chunks[3] != nullptr)
						chunks[interactionChunkIndex]->chunks[3]->updateMesh();
					return;
				}
			}
		}
	}
}

void Player::updateCamera()
{
	camera.update(glm::vec3(position.x, position.y + PLAYER_SIZE_Y / 2 - 0.13f, position.z));
}

bool Player::axisCollision(float one, float two, float parameter)
{
	return (one - (two + parameter)) * (one - (two - parameter)) <= 0;
}

void Player::applyMotion(glm::vec3 motion, const std::vector<Chunk*>& chunks)
{
	if (motion.y <= -1.0f)
		motion.y = -1.0f;

	bool overallCollisionY = false;

	//a little problem with block's angles, cause when i need to fall for some reason i stand in the air
	for (int j = 0; j < chunks.size(); j++) {
		if (chunks[j] != nullptr) {
			for (int i = 0; i < CHUNK_SIZE; i++)
			{
				if (chunks[j]->coordinate[i].type == GRASS || chunks[j]->coordinate[i].type == STONE) {
					glm::vec3 coord = chunks[j]->coordinate[i].coord;

					if (motion.x != 0.0f) {
						glm::vec3 top = glm::vec3(position.x + motion.x + PLAYER_SIZE_X / 2, position.y + PLAYER_SIZE_Y / 2, position.z + PLAYER_SIZE_Z / 2);
						glm::vec3 bottom = glm::vec3(position.x + motion.x - PLAYER_SIZE_X / 2, position.y - PLAYER_SIZE_Y / 2, position.z - PLAYER_SIZE_Z / 2);

						bool collisionX = axisCollision(top.x, coord.x, 0.5f) || axisCollision(bottom.x, coord.x, 0.5f) || axisCollision(coord.x, position.x + motion.x, PLAYER_SIZE_X / 2);
						bool collisionY = axisCollision(top.y, coord.y, 0.5f) || axisCollision(bottom.y, coord.y, 0.5f) || axisCollision(coord.y, position.y, PLAYER_SIZE_Y / 2);
						bool collisionZ = axisCollision(top.z, coord.z, 0.5f) || axisCollision(bottom.z, coord.z, 0.5f) || axisCollision(coord.z, position.z, PLAYER_SIZE_Z / 2);

						if (collisionX && collisionY && collisionZ)
							motion.x = 0.0f;
					}
					if (motion.y != 0.0f) {
						glm::vec3 top = glm::vec3(position.x + PLAYER_SIZE_X / 2, position.y + motion.y + PLAYER_SIZE_Y / 2, position.z + PLAYER_SIZE_Z / 2);
						glm::vec3 bottom = glm::vec3(position.x - PLAYER_SIZE_X / 2, position.y + motion.y - PLAYER_SIZE_Y / 2, position.z - PLAYER_SIZE_Z / 2);

						bool collisionX = axisCollision(top.x, coord.x, 0.5f) || axisCollision(bottom.x, coord.x, 0.5f) || axisCollision(coord.x, position.x, PLAYER_SIZE_X / 2);
						bool collisionY = axisCollision(top.y, coord.y, 0.5f) || axisCollision(bottom.y, coord.y, 0.5f) || axisCollision(coord.y, position.y + motion.y, PLAYER_SIZE_Y / 2);
						bool collisionZ = axisCollision(top.z, coord.z, 0.5f) || axisCollision(bottom.z, coord.z, 0.5f) || axisCollision(coord.z, position.z, PLAYER_SIZE_Z / 2);

						if (collisionX && collisionY && collisionZ) {
							if (coord.y + 0.5f <= position.y + motion.y && state != FLYING)
								state = STANDING;
							else if (position.y + motion.y <= coord.y - 0.5f && state != FLYING)
								state = MIDAIR;

							motion.y = 0.0f;
							yVelocity = 0.0f;
							overallCollisionY = true;
						}
					}
					if (motion.z != 0.0f) {
						glm::vec3 top = glm::vec3(position.x + PLAYER_SIZE_X / 2, position.y + PLAYER_SIZE_Y / 2, position.z + motion.z + PLAYER_SIZE_Z / 2);
						glm::vec3 bottom = glm::vec3(position.x - PLAYER_SIZE_X / 2, position.y - PLAYER_SIZE_Y / 2, position.z + motion.z - PLAYER_SIZE_Z / 2);

						bool collisionX = axisCollision(top.x, coord.x, 0.5f) || axisCollision(bottom.x, coord.x, 0.5f) || axisCollision(coord.x, position.x, PLAYER_SIZE_X / 2);
						bool collisionY = axisCollision(top.y, coord.y, 0.5f) || axisCollision(bottom.y, coord.y, 0.5f) || axisCollision(coord.y, position.y, PLAYER_SIZE_Y / 2);
						bool collisionZ = axisCollision(top.z, coord.z, 0.5f) || axisCollision(bottom.z, coord.z, 0.5f) || axisCollision(coord.z, position.z + motion.z, PLAYER_SIZE_Z / 2);

						if (collisionX && collisionY && collisionZ)
							motion.z = 0.0f;
					}
				}
			}
		}
	}

	if (!overallCollisionY && state != FLYING)
		state = MIDAIR;

	position += motion;
}

int* Player::getOrder()
{
	int* order = new int[3];
	glm::vec3 absCameraFront = glm::vec3(abs(camera.Front.x), abs(camera.Front.y), abs(camera.Front.z));

	// 1 - Ox; 2 - Oy; 3 - Oz;
	if (absCameraFront.x >= absCameraFront.y && absCameraFront.x >= absCameraFront.z) {
		order[0] = 1;
		if (absCameraFront.y >= absCameraFront.z) {
			order[1] = 2;
			order[2] = 3;
		}
		else {
			order[1] = 3;
			order[2] = 2;
		}
	}
	else if (absCameraFront.y >= absCameraFront.x && absCameraFront.y >= absCameraFront.z) {
		order[0] = 2;
		if (absCameraFront.x >= absCameraFront.z) {
			order[1] = 1;
			order[2] = 3;
		}
		else {
			order[1] = 3;
			order[2] = 1;
		}
	}
	else {
		order[0] = 3;
		if (absCameraFront.x >= absCameraFront.y) {
			order[1] = 1;
			order[2] = 2;
		}
		else {
			order[1] = 2;
			order[2] = 1;
		}
	}
	return order;
}

Player::~Player()
{
	delete shader;
	delete texture;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

int sgn(float a)
{
	if (a > 0.0f)
		return 1;
	else if (a < 0.0f)
		return -1;
	else return 0;
}