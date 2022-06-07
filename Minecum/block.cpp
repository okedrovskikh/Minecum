#include "block.h"

Block::Block()
{

}

Block::Block(std::string vertexPath, std::string fragmentPath, std::string texturePath, BlockType type)
{
	this->shader = new Shader(vertexPath.c_str(), fragmentPath.c_str());
	this->texture = new Texture(texturePath);
    this->type = type;

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

void Block::draw()
{

}

Block::~Block()
{
    delete shader;
    delete texture;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

glm::vec3 operator* (glm::vec3 f, float s)
{
    return glm::vec3(f.x * s, f.y * s, f.z * s);
}
glm::vec3 operator* (float f, glm::vec3 s)
{
    return glm::vec3(s.x * f, s.y * f, s.z * f);
}