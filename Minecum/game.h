#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>

#include "player.h"
#include "shader.h"
#include "block.h"
#include "chunk.h"
#include "world.h"
#include "camera.h"
#include "texture.h"
#include "crosshair.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

GLFWwindow* windowInit(int width, int height, std::string name);
void window_size_callback(GLFWwindow* window, int width, int height);
Block** initBlocks();