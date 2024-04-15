// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <boost/asio.hpp>

#include "../Messages/msg_export.h"
#include "../Networking/Agent.h"
#include "../Utils/Config.h"
#include "../Logic/ClientLogic.h"
#include <chrono>
#include <sys/timeb.h>
#include <ctime>
#include <thread>
#include "cstdlib"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
// Window dimensions
const GLuint WIDTH = 1940, HEIGHT = 1100;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
                                   "uniform vec3 sdvig;\n"
                                   "layout (location = 0) in vec3 position;\n"
                                   "void main()\n"
                                   "{\n"
                                   "gl_Position = vec4(position.x+sdvig.x, position.y+sdvig.y, position.z+sdvig.z, 1.0);\n"
                                   "}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
                                     "out vec4 color;\n"
                                     "void main()\n"
                                     "{\n"
                                     "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                     "}\n\0";

const GLchar* fragmentShaderSource1 = "#version 330 core\n"
                                      "out vec4 color;\n"
                                      "void main()\n"
                                      "{\n"
                                      "color = vec4(0.0f, 0.5f, 0.2f, 1.0f);\n"
                                      "}\n\0";

GLfloat Vertex[90];
void CreateTR(int ind,GLfloat A,GLfloat B)
{
    Vertex[9*ind]=A;
    Vertex[9*ind+1]=B;
    Vertex[9*ind+2]=0;

    Vertex[9*ind+3]=A+0.05;
    Vertex[9*ind+4]=B+0.2;
    Vertex[9*ind+5]=0;

    Vertex[9*ind+6]=A+0.1;
    Vertex[9*ind+7]=B;
    Vertex[9*ind+8]=0;
}

GLfloat x=0;
GLfloat y=0;
GLfloat z=0;

int main()
{
    // INIT
    ClientLogic client;
    auto ns = std::chrono::high_resolution_clock::now();
        std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
        // Init GLFW
        glfwInit();
        // Set all the required options for GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, key_callback);
        glewInit();


        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);


        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);


        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        GLuint fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
        glCompileShader(fragmentShader);
        glCompileShader(fragmentShader1);

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        GLuint shaderProgram1 = glCreateProgram();
        glAttachShader(shaderProgram1, vertexShader);
        glAttachShader(shaderProgram1, fragmentShader1);
        glLinkProgram(shaderProgram1);



        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteShader(fragmentShader1);


        GLfloat firstTriangle[] = {
                -0.1f, -0.1f, 0.0f,  // Left
                -0.0f, -0.1f, 0.0f,  // Right
                -0.05f, 0.1f, 0.0f,  // Top
        };
        GLfloat secondTriangle[] = {
                0.0f, -0.5f, 0.0f,  // Left
                0.9f, -0.5f, 0.0f,  // Right
                0.45f, 0.5f, 0.0f   // Top
        };


        GLuint VBO[2], VAO[2];
        glGenVertexArrays(2, VAO); // We can also generate multiple VAOs or buffers at the same time
        glGenBuffers(2, VBO);
        //glGenBuffers(1, &EBO);
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray(VAO[0]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

        GLint Location = glGetUniformLocation(shaderProgram, "sdvig");

        x=0;
        y=0;
        z=0;

    // MAIN PART HERE

    std::map<std::pair<std::string, unsigned short>, std::vector<GLfloat>> data = {};
    std::cout << "[LOG] Starting message dispatch loop.." << std::endl;
    long lastTime =  std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count();
    double nsPerTick = 1000000000/60.0f;

    int ticks = 0;
    double delta = 0;
    while (!glfwWindowShouldClose(window) && client.isGood())
    {
        ns = std::chrono::high_resolution_clock::now();
        //Get time in nano seconds
        long now = std::chrono::duration_cast<std::chrono::nanoseconds>
                (std::chrono::system_clock::now().time_since_epoch()).count();
        delta += (now-lastTime)/nsPerTick;
        lastTime = now;

        while (delta >= 1){
            ticks++;
            client.DispatchMessage(data, x, y, z);
            delta--;
        }

        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderProgram);
        glUniform3f(Location, x,y,z);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(VAO[1]);


        for (auto& [key, value] : data)
        {
            CreateTR(0,value[0],value[1]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), Vertex, GL_STREAM_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            glUseProgram(shaderProgram1);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;
        }

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glfwTerminate();

    // OUTRO

    std::cout << "[LOG] exiting" << std::endl;
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
        y+=0.01;

    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
        y-=0.01;

    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        x-=0.01;

    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        x+=0.01;
}