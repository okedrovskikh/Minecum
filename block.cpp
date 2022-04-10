#include "block.h"

BlockPrototype::BlockPrototype()
{

}

BlockPrototype::BlockPrototype(unsigned int& VAO, unsigned int& VBO)
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

BlockPrototype::~BlockPrototype()
{

}

Block::Block()
{

}

Block::Block(unsigned int& VAO, unsigned int& VBO, const Shader& shader, const Texture& texture) : geometry(VAO, VBO)
{
	this->shader = shader;
	this->texture = texture;
}

Block::~Block()
{

}