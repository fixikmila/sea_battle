#ifndef SHIPS_CLIENTGUI_H
#define SHIPS_CLIENTGUI_H

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <map>
#include <vector>
#include <string>

class ClientGui {
public:
    ClientGui(void (*key_callback)(GLFWwindow*, int, int, int, int));
    ~ClientGui();
    bool isGood();
    void DrawFrame(std::map<std::pair<std::string, unsigned short>, std::vector<GLfloat>>&data, GLfloat x, GLfloat y, GLfloat z);
private:
    // Window dimensions
    const GLuint WIDTH = 1940, HEIGHT = 1100;
    GLFWwindow* window;
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
    void CreateTR(int ind,GLfloat A,GLfloat B);
    GLuint vertexShader, fragmentShader, fragmentShader1, shaderProgram, shaderProgram1;
    GLuint VBO[2], VAO[2];
    GLint Location;
};


#endif //SHIPS_CLIENTGUI_H
