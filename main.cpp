#include "game.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = WIDTH / 2;
float lastY = HEIGHT / 2;

Player player;

Chunk chunk;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    player.processMovement(window, deltaTime, chunk.coordinate);
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
        player.processLeftClick(chunk.coordinate);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        player.processRightClick(chunk.coordinate);
}

int main()
{
    GLFWwindow* window = windowInit(WIDTH, HEIGHT, "minecum");

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);


    unsigned int VBOs[3], VAOs[3];

    Shader containerShader("vertexContainer.glsl", "fragmentContainer.glsl");
    BlockPrototype container(VAOs[0], VBOs[0]);

    Texture containerTexture = Texture("container.jpg");
    containerShader.use();
    containerShader.setInt("texture1", 1);

    Shader playerShader("playerVertex.glsl", "playerFragment.glsl");
    player = Player(glm::vec3(0.0f, 3.0f, 0.0f), VAOs[1], VBOs[1]);

    Texture playerTexture = Texture("flAOQJ7reKc.jpg");
    playerShader.use();
    playerShader.setInt("playerTexture", 0);

    Shader crosshairShader("crosshairVertex.glsl", "crosshairFragment.glsl");
    Crosshair crosshair(VAOs[2], VBOs[2]);



    while (!glfwWindowShouldClose(window))
    {   
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, playerTexture.ID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, containerTexture.ID);
        
        //chunk draw
        {
            containerShader.use();
            glm::mat4 projection = glm::perspective(glm::radians(100.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
            containerShader.setMat4("projection", projection);
            glm::mat4 view = player.camera.getViewMatrix();
            containerShader.setMat4("view", view);
            glBindVertexArray(VAOs[0]);
            int size = chunk.coordinate.size();
            for (int i = 0; i < size; i++) 
            {
                glm::vec3 coord = chunk.coordinate[i];
                
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, coord);
                containerShader.setMat4("model", model);

                if (false)
                    containerShader.setFloat("chosen", 0.2f);
                else
                    containerShader.setFloat("chosen", 1.0f);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

        }

        //crosshair draw
        {
            crosshairShader.use();
            glBindVertexArray(VAOs[2]);
            glDrawArrays(GL_TRIANGLES, 0, 12);
        }

        //player draw
        /*{
            playerShader.use();
            glm::mat4 projection = glm::perspective(glm::radians(100.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
            playerShader.setMat4("projection", projection);
            glm::mat4 view = player.camera.GetViewMatrix();
            playerShader.setMat4("view", view);
            glBindVertexArray(VAOs[1]);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, player.position);
            playerShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }*/
 
        //debug
        //std::cout << player.position.x << " " << player.position.y << " " << player.position.z << std::endl;
        
        processInput(window);
        player.updateCamera();
        
        //std::cout << "-------------------------------" << std::endl;
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    glfwTerminate();

	return 0;
}