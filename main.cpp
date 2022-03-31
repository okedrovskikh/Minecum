#include "player.h"
#include "shader.h"
#include "block.h"
#include "chunk.h"
#include "camera.h"
#include "collider.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "texture.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = WIDTH / 2;
float lastY = HEIGHT / 2;

Player player;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        player.ProcessKeyboard(UP, deltaTime);
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

    player.camera.ProcessMouseMovement(xoffset, yoffset);
}

int main()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "minecum", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    Chunk chunk;
    unsigned int VBOs[2], VAOs[2];
    Shader containerShader("vertexContainer.glsl", "fragmentContainer.glsl");
    Block container(VAOs[0], VBOs[0]);
    Shader playerShader("playerVertex.glsl", "playerFragment.glsl");
    player = Player(glm::vec3(0.0f, 0.5f, 0.0f), VAOs[1], VBOs[1]);

    Texture playerTexture = Texture("flAOQJ7reKc.jpg");
    playerShader.use();
    playerShader.setInt("playerTexture", 0);

    Texture containerTexture = Texture("container.jpg");
    containerShader.use();
    containerShader.setInt("texture1", 1);

    while (!glfwWindowShouldClose(window))
    {
        player.setFly(false);
        bool fall = true;
        glm::vec4 closestPlain;
        glm::vec3 currPosition = player.entity.position;
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, playerTexture.ID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, containerTexture.ID);
        
        {
            containerShader.use();
            glm::mat4 projection = glm::perspective(glm::radians(100.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
            containerShader.setMat4("projection", projection);
            glm::mat4 view = player.camera.GetViewMatrix();
            containerShader.setMat4("view", view);
            glBindVertexArray(VAOs[0]);
            glm::mat4 model;
            for (int i = 0; i < 29; i++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, chunk.coordinate[i]);
                containerShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                Collider currBlockCollider(chunk.coordinate[i], 1.0f, 1.0f, 1.0f, container.getVertices(), container.getVerticesSize());
                bool collisionX = false;
                bool collisionY = false;
                bool collisionZ = false;
                player.checkCollisionAABB(collisionX, collisionY, collisionZ, currBlockCollider);
                if (collisionX && collisionY && collisionZ)
                {
                    std::cout << "Potential collision with " << chunk.coordinate[i].x << " " << chunk.coordinate[i].y << " " << chunk.coordinate[i].z << std::endl;
                    //std::cout << currBlockCollider.distance(player.entity.position) << std::endl;
                    closestPlain = currBlockCollider.getClosestPlain(player.entity.position);
                    //std::cout << closestPlain.x << " " << closestPlain.y << " " << closestPlain.z << " " << closestPlain.w << std::endl;
                    float distToClosestPlain = currBlockCollider.getDistToPlain(closestPlain, player.entity.position);
                    if (distToClosestPlain <= 0.30f) {
                        std::cout << "Near plain " << closestPlain.x << " " << closestPlain.y << " " << closestPlain.z << " " << closestPlain.w << std::endl;
                        
                        //player.entity.position = currPosition;
                        if (closestPlain.x != 0){
                            
                        }
                        if (closestPlain.y != 0){
                            player.entity.position.y -= 0.3f * closestPlain.y;
                            fall = false;
                        }
                        if (closestPlain.z != 0) {

                        }
                    }
                }
            }
        }

        //debug
        //std::cout << player.camera.Position.x << " " << player.camera.Position.y << " " << player.camera.Position.z << std::endl;
        std::cout << player.entity.position.x << " " << player.entity.position.y << " " << player.entity.position.z << std::endl;
        
        //player draw
        /*{
            playerShader.use();
            glm::mat4 projection = glm::perspective(glm::radians(100.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
            playerShader.setMat4("projection", projection);
            glm::mat4 view = player.camera.GetViewMatrix();
            playerShader.setMat4("view", view);
            glBindVertexArray(VAOs[1]);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, player.entity.position);
            playerShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }*/

        player.fall(fall, deltaTime);
        player.entity.updateCollider();
        player.camera.update(glm::vec3(player.entity.position.x, player.entity.position.y + player.entity.height / 2, player.entity.position.z));
        
        std::cout << "-------------------------------" << std::endl;
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	return 0;
}