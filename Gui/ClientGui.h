#ifndef SHIPS_CLIENTGUI_H
#define SHIPS_CLIENTGUI_H

#include "atomic"
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <map>
#include <vector>
#include <string>
#include "../Graphics/Shader.h"
#include "../Graphics/Camera.h"
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ClientGui {
public:
    ClientGui();
    static void Init();
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    ~ClientGui();
    static bool isGood();
    static void DrawFrame(std::map<std::pair<std::string, unsigned short>, std::vector<GLuint>>&data,std::map<std::pair<std::string, unsigned short>, std::vector<GLuint>>&data_cubes);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void Do_Movement();
    static glm::mat4 E();
    static GLuint x,y,z,flag;
    static GLfloat fov;
    static void Finish();
    static int r,g,b;
    static int was_deleted;
private:
    static std::map<float,int> MAP;
    static std::map<int,float> ALP;
    static int used[6][6][6],type[6][6][6],move[6][3];
    static float ror[6][4],timedel[6][6][6];
    static GLuint screenWidth, screenHeight;
// Camera
    static Camera* camera;
    static bool keys[1024];
    static GLfloat lastX, lastY;
    static std::atomic<bool> firstMouse;

    static GLfloat deltaTime ;
    static GLfloat lastFrame ;
    // Window dimensions
    static int width, height;
    static GLFWwindow* window;
    static GLuint VBO, VAO,VAONORM,VBONORM;
    static GLuint texture1;
    static GLuint texture2;
    static std::vector< std::pair< std::pair<GLfloat,GLfloat>,GLfloat > >kuby;
    static Shader* ourShader;
};


#endif //SHIPS_CLIENTGUI_H
