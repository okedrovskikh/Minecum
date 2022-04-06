#pragma once
#include "player.h"
#include "shader.h"
#include "block_prototype.h"
#include "chunk.h"
#include "camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "texture.h"
#include "crosshair.h"

GLFWwindow* windowInit(int width, int height, std::string name);