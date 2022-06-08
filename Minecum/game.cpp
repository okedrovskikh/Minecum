#include "game.h"

GLFWwindow* windowInit(int width, int height, std::string name)
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    return window;
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Block** initBlocks()
{
    Block** blocks = new Block*[3];

    blocks[0] = new Block("../shader/grassVertex.glsl", "../shader/grassFragment.glsl", "../texture/grass.jpg", GRASS);
    blocks[0]->shader->use();
    blocks[0]->shader->setInt("grassTexture", 0);

    blocks[1] = new Block("../shader/stoneVertex.glsl", "../shader/stoneFragment.glsl", "../texture/stone.jpg", STONE);
    blocks[1]->shader->use();
    blocks[1]->shader->setInt("stoneTexture", 1);

    blocks[2] = new Block("../shader/sandVertex.glsl", "../shader/sandFragment.glsl", "../texture/sand.jpg", SAND);
    blocks[2]->shader->use();
    blocks[2]->shader->setInt("sandtexture", 2);

    return blocks;
}