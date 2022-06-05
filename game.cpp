#include "game.h"

GLFWwindow* windowInit(int width, int height, std::string name)
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        return NULL;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    return window;
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Block** initBlocks()
{
    Block** blocks = new Block*[2];

    blocks[0] = (new Block("grassVertex.glsl", "grassFragment.glsl", "grass.jpg", GRASS));
    blocks[0]->shader->use();
    blocks[0]->shader->setInt("grassTexture", 0);

    blocks[1] = (new Block("stoneVertex.glsl", "stoneFragment.glsl", "stone.jpg", STONE));
    blocks[1]->shader->use();
    blocks[1]->shader->setInt("stoneTexture", 1);

    return blocks;
}