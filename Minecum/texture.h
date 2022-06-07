#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Texture
{
public:
	unsigned int ID;
	Texture();
	Texture(std::string textureName);
	~Texture();
private:
};