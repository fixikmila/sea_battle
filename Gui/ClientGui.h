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
#include <string>

class ClientGui {
public:
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

    ClientGui();
    static void reset();
    static void Init();
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    ~ClientGui();
    static bool isGood();
    static void DrawFrame(std::map<std::pair<std::string, unsigned short>, std::vector<GLuint>>&data,std::map<std::pair<std::string, unsigned short>, std::vector<GLuint>>&data_cubes);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void Do_Movement();
    static void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color, float dlin);
    static bool in_game;
    static glm::mat4 E();
    static GLuint x,y,z,flag,F;
    static GLfloat fov;
    static void Finish();
    static int r,g,b,del,mydel;
    static int was_deleted;
    static bool wrong;
    static float gamesound, musicsound;
    static bool esc_waiting;
    static int used[6][6][6],type[6][6][6],move[6][3];
private:
    static std::map<GLchar, Character> Characters;
    static std::map<float,int> MAP;
    static std::map<int,float> ALP;
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
    static GLuint VBO, VAO,VAONORM,VBONORM,vao,vbo,VAO1,VBO1;
    static GLuint texture1;
    static GLuint texture2;
    static GLuint texture3;

    static std::vector<GLfloat> grom;
    static std::vector<std::pair<GLfloat,GLfloat > > strelki,tigi;
    static std::vector< std::pair< std::pair<GLfloat,GLfloat>,GLfloat > >kuby;
    static std::vector< std::pair< std::pair<GLfloat,GLfloat>, std::pair<GLfloat,GLfloat> > > knopki;
    static std::vector<std::string> stroki;
    static Shader* ourShader;
    static Shader* KnopkiShader;
    static Shader* TextShader;
};


#endif //SHIPS_CLIENTGUI_H
