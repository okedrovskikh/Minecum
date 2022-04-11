#include "game.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = WIDTH / 2;
float lastY = HEIGHT / 2;

Player player;

Chunk chunk(glm::vec3(-3.0f, -14.0f, -3.0f));

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    player.processMovement(window, deltaTime, chunk);
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

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        player.processLeftClick(chunk);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        player.processRightClick(chunk);
}

int main()
{
    GLFWwindow* window = windowInit(WIDTH, HEIGHT, "minecum");

    if (window == NULL)
        return -1;

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);


    unsigned int VBOs[3], VAOs[3];

    Shader containerShader("vertexContainer.glsl", "fragmentContainer.glsl");
    Texture containerTexture("container.jpg");
    Block container(VAOs[0], VBOs[0], containerShader, containerTexture);
    container.shader.use();
    container.shader.setInt("texture1", 0);

    Shader playerShader("playerVertex.glsl", "playerFragment.glsl");
    Texture playerTexture("flAOQJ7reKc.jpg");
    player = Player(glm::vec3(0.0f, 5.0f, 0.0f), VAOs[1], VBOs[1], playerShader, playerTexture);
    player.shader.use();
    player.shader.setInt("playerTexture", 1);

    Shader crosshairShader("crosshairVertex.glsl", "crosshairFragment.glsl");
    Crosshair crosshair(VAOs[2], VBOs[2], crosshairShader);
    crosshair.shader.use();


    while (!glfwWindowShouldClose(window))
    {   
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, container.texture.ID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, player.texture.ID);
        
        //chunk draw
        {
            container.shader.use();
            glm::mat4 projection = glm::perspective(glm::radians(100.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
            container.shader.setMat4("projection", projection);
            glm::mat4 view = player.camera.getViewMatrix();
            container.shader.setMat4("view", view);
            glBindVertexArray(VAOs[0]);
            for (int i = 0; i < SIZE; i++) 
            {
                if (chunk.coordinate[i].second.first != AIR) {
                    glm::vec3 coord = chunk.coordinate[i].first;

                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, coord);
                    container.shader.setMat4("model", model);

                    if (chunk.coordinate[i].second.second == true)
                        container.shader.setFloat("chosen", 0.6f);
                    else
                        container.shader.setFloat("chosen", 1.0f);

                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        }

        //player draw
        //lol, player are not drawing correctly
        /*{
            player.shader.use();
            glm::mat4 projection = glm::perspective(glm::radians(100.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
            player.shader.setMat4("projection", projection);
            glm::mat4 view = player.camera.getViewMatrix();
            player.shader.setMat4("view", view);
            glm::mat4 model = glm::mat4(1.0f);
            glBindVertexArray(VAOs[1]);
            model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
            player.shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }*/

        //crosshair draw
        {
            crosshair.shader.use();
            glBindVertexArray(VAOs[2]);
            glDrawArrays(GL_TRIANGLES, 0, 12);
        }
 
        //debug
        //std::cout << player.position.x << " " << player.position.y << " " << player.position.z << std::endl;
        
        processInput(window);
        player.updateCamera();
        player.rayCast(chunk);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(3, VAOs);
    glDeleteBuffers(3, VBOs);

    glfwTerminate();

	return 0;
}