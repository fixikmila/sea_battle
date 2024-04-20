#include <iostream>
#include "ClientGui.h"

ClientGui::ClientGui(void (*key_callback)(GLFWwindow*, int, int, int, int))
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glewInit();


    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);


    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
    glCompileShader(fragmentShader);
    glCompileShader(fragmentShader1);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    shaderProgram1 = glCreateProgram();
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


    VBO[2], VAO[2];
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

    Location = glGetUniformLocation(shaderProgram, "sdvig");
}

bool ClientGui::isGood() {
    return !glfwWindowShouldClose(window);
}

void ClientGui::DrawFrame(std::map<std::pair<std::string, unsigned short>, std::vector<GLfloat>>&data, GLfloat x, GLfloat y, GLfloat z)
{
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

ClientGui::~ClientGui()
{
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glfwTerminate();
}

void ClientGui::CreateTR(int ind, GLfloat A, GLfloat B) {
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
