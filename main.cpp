#include "game.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = WIDTH / 2;
float lastY = HEIGHT / 2;

Player* player;

World world;
std::vector<Chunk*> chunks;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    player->camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        player->processLeftClick(chunks);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        player->processRightClick(chunks);
}

int main()
{
    GLFWwindow* window = windowInit(WIDTH, HEIGHT, "minecum");

    if (window == NULL)
        return -1;

    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    std::vector<Block*> blocks = initBlocks();

    player = new Player(glm::vec3(-6.0f, 16.0f, -6.0f), "playerVertex.glsl", "playerFragment.glsl", "flAOQJ7reKc.jpg");
    player->shader->use();
    player->shader->setInt("playerTexture", 2);

    Crosshair crosshair("crosshairVertex.glsl", "crosshairFragment.glsl");
    crosshair.shader->use();

    int textureID = GL_TEXTURE0;
    glActiveTexture(textureID);
    glBindTexture(GL_TEXTURE_2D, blocks[0]->texture->ID);
    glActiveTexture(textureID + 1);
    glBindTexture(GL_TEXTURE_2D, blocks[1]->texture->ID);
    glActiveTexture(textureID + 2);
    glBindTexture(GL_TEXTURE_2D, player->texture->ID);



    while (!glfwWindowShouldClose(window))
    {   
        //data
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //fps
        //std::cout << "\r" << "fps " << round(1 / deltaTime) << " " << std::flush;

        //update player
        processInput(window);
        chunks = world.getChunks(player->camera.Position, player->camera.Position + INTERACTION_RADIUS * player->camera.Front);
        player->rayCast(chunks);
        player->processMovement(window, deltaTime, world);
        player->updateCamera();

        //debug
        //std::cout << player.position.x << " " << player.position.y << " " << player.position.z << " " << std::flush;


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //world draw
        glm::mat4 view = player->camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(100.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        for (int k = 0; k < blocks.size(); k++)
        {
            glBindVertexArray(blocks[k]->VAO);
            blocks[k]->shader->use();
            blocks[k]->shader->setMat4("projection", projection);
            blocks[k]->shader->setMat4("view", view);
            for (int i = 0; i < WORLD_SIZE; i++)
            {
                for (int j = 0; j < world.chunk[i]->mesh.size(); j++)
                {
                    if (world.chunk[i]->mesh[j]->second.type == blocks[k]->type) {
                        blocks[k]->shader->setMat4("model", glm::translate(glm::mat4(1.0f), world.chunk[i]->mesh[j]->first));

                        if (world.chunk[i]->mesh[j]->second.chosen == true)
                            blocks[k]->shader->setFloat("chosen", 0.6f);
                        else
                            blocks[k]->shader->setFloat("chosen", 1.0f);

                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    }
                }
            }
        }

        //player draw

        //crosshair draw
        crosshair.draw();
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    delete player;

	return 0;
}