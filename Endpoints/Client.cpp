#include <iostream>
#include <chrono>
#include <thread>

#include "../Messages/msg_export.h"
#include "../Networking/Agent.h"
#include "../Logic/ClientLogic.h"
#include "../Gui/ClientGui.h"

GLfloat x=0;
GLfloat y=0;
GLfloat z=0;

const GLfloat step = 0.01; // 0.01

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
        y+=step;

    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
        y-=step;

    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        x-=step;

    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        x+=step;
}

int main()
{
    ClientLogic client;
    ClientGui gui(key_callback);

    std::map<std::pair<std::string, unsigned short>, std::vector<GLfloat>> data = {};
    std::cout << "[LOG] Starting message dispatch loop.." << std::endl;

    const std::chrono::duration<float> targetFrameTime(std::chrono::duration<float>(1.0f / CLIENT_FPS));
    auto lastFrameTime = std::chrono::high_resolution_clock::now();

    while (gui.isGood() && client.isGood())
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = currentTime - lastFrameTime;
        if (elapsedTime < targetFrameTime)
        {
            std::this_thread::sleep_for(targetFrameTime - elapsedTime);
            currentTime = std::chrono::high_resolution_clock::now();
            elapsedTime = currentTime - lastFrameTime;
        }

        client.DispatchMessage(data, x, y, z);
        gui.DrawFrame(data, x, y, z);

        lastFrameTime = currentTime;

    }

    std::cout << "[LOG] exiting" << std::endl;
    return 0;
}