#pragma once
#include "shader.h"
#include "entity.h"
#include "camera.h"
#include "texture.h"
#include "block.h"
#include <limits>

const float WIDTH_X = 0.6f;
const float HEIGHT_Y = 0.6f;
const float WIDTH_Z = 0.6f;

enum Movement {
	FORWARD, BACKWARD, LEFT, RIGHT, UP
};

class Player
{
public:
	//Shader shader;
	//Texture texture;
	Entity entity;
	Camera camera;
	Player();
    Player(glm::vec3 position, unsigned int& VAO, unsigned int& VBO);
    void ProcessKeyboard(Movement direction, float deltaTime);
    void checkCollisionAABB(bool& collisionX, bool& collisionY, bool& collisionZ, Collider other);
    void fall(bool fall, float deltaTime);
    void setFly(bool set);
    ~Player();
private:
    bool fly;
    bool grounded;
    float vertices[288] = {
        -0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

         0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f, -0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.3f, -0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        -0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.3f,  0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.3f,  0.3f, -0.3f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };
};