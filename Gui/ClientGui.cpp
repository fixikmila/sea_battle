#include <iostream>
#include "ClientGui.h"
#include <atomic>
#include <map>
#include <iomanip>

#include <ft2build.h>
#include FT_FREETYPE_H
std::chrono::steady_clock::time_point ClientGui::begin = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point ClientGui::end = std::chrono::steady_clock::now();
bool ClientGui::esc_waiting = false;
float ClientGui::gamesound = 0.5f;
float ClientGui::musicsound = 0.5f;
float ClientGui::dx = 3;
float ClientGui::dy= 3;
float ClientGui::dz = 3;

bool ClientGui::wrong = false;
bool ClientGui::in_game = false;
GLuint ClientGui::x = 5;
GLuint ClientGui::y = 5;
GLuint ClientGui::z = 5;

int ClientGui::r = 0;
int ClientGui::g = 0;
int ClientGui::b = 0;
int ClientGui::del = 0;
int ClientGui::mydel = 0;
int ClientGui::was_deleted = 0;
GLuint ClientGui::flag = 0;
GLuint ClientGui::F = 1;
GLfloat ClientGui::fov = 45.0f;
GLfloat ClientGui::lastX = 0.0f;
GLfloat ClientGui::lastY = 0.0f;
std::atomic<bool> ClientGui::firstMouse = false;
bool ClientGui::keys[1024] = {};
std::map<float,int> ClientGui::MAP = {};
std::map<int,float> ClientGui::ALP = {};
std::map<GLchar,ClientGui::Character> ClientGui::Characters;
Camera* ClientGui::camera = nullptr;
GLFWwindow* ClientGui::window = nullptr;
GLfloat ClientGui::deltaTime = {};
GLfloat ClientGui::lastFrame = {};
GLfloat ClientGui::lastSend = 0;
GLuint ClientGui::VAO = {};
GLuint ClientGui::VBO = {};
GLuint ClientGui::VAO1 = {};
GLuint ClientGui::VBO1 = {};
GLuint ClientGui::vao = {};
GLuint ClientGui::vbo = {};
GLuint ClientGui::VAONORM = {};
GLuint ClientGui::VBONORM = {};
int ClientGui::width = {};
int ClientGui::height = {};
GLuint ClientGui::screenHeight = {};
GLuint ClientGui::screenWidth = {};
std::vector< std::pair< std::pair<GLfloat,GLfloat>,GLfloat > > ClientGui::kuby = {};
std::vector< std::pair< std::pair<GLfloat,GLfloat>,std::pair<GLfloat,GLfloat> > > ClientGui::knopki = {};
std::vector<std::pair<GLfloat,GLfloat > > ClientGui::strelki= {};
std::vector<std::pair<GLfloat,GLfloat > > ClientGui::tigi= {};
std::vector<std::string> ClientGui::stroki= {};
std::vector<GLfloat> ClientGui::grom={};
Shader* ClientGui::ourShader = nullptr;
Shader* ClientGui::KnopkiShader = nullptr;
Shader* ClientGui::TextShader = nullptr;
Shader* ClientGui::TShader = nullptr;
Shader* ClientGui::SpecShader = nullptr;

GLuint ClientGui::texture1 = {};
GLuint ClientGui::texture2 = {};
GLuint ClientGui::texture3 = {};
GLuint ClientGui::texture4 = {};
GLuint ClientGui::texture5 = {};

int ClientGui::used[6][6][6] = {{0,0,0},{0,0,0},{0,0,0}};
int ClientGui::type[6][6][6] = {{0,0,0},{0,0,0},{0,0,0}};
int ClientGui::move[6][3]={{0,-1,0},{0,1,0},{0,0,-1} ,{0,0,1},{-1,0,0},{1,0,0} };
GLfloat p = glm::pi<float>();
float ClientGui::ror[6][4]={ {0,0,0,0},{0,1,0,0},{0,1,0,0} ,{0,1,0,0},{0,0,0,1},{0,0,0,1} };
float ClientGui::timedel[6][6][6] = {{0,0,0},{0,0,0},{0,0,0}};

ClientGui::ClientGui() {

}

void ClientGui::RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color, float dlin)
{
    // activate corresponding render state


    float start=x;
    shader.use();
    //std::cout<<shader.ID<<std::endl;
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(ClientGui::vao);

    // iterate through all characters
    std::string::const_iterator c;
    float first[6][4],last[6][4];
    int Fl=0;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        //std::cout<<*c<<" "<<ch.TextureID<<" "<<ch.Size.x<<" "<<ch.Size.y<<std::endl;
        // update VBO for each character

       // std::cout<<x<<" "<<y<<" "<<w<<" "<<h<<std::endl;
        float byk[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        for(int hh=0;hh<6;hh++)
        {
            for(int kk=0;kk<4;kk++)
            {
                last[hh][kk]=byk[hh][kk];
            }
        }

        if(Fl==0)
        {
            for(int hh=0;hh<6;hh++)
            {
                for(int kk=0;kk<4;kk++)
                {
                    first[hh][kk]=byk[hh][kk];
                }
            }
        }
        Fl=1;
        // render glyph texture over quad
        //std::cout<<ch.TextureID<<std::endl;

        //ch.TextureID

        if(F==0)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture5);
            glUniform1i(glGetUniformLocation(shader.ID, "text"), 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            glUniform1i(glGetUniformLocation(shader.ID, "T"), 1);

        }
        else
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            glUniform1i(glGetUniformLocation(shader.ID, "text"), 0);
        }

        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(byk), byk); // be sure to use glBufferSubData and not glBufferData

        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        //std::cout<<"x="<<x<<" start="<<start<<std::endl;
        if(x-start>1.5 && *c==' ')
        {
            x=start;
            y-=0.2;
        }
    }

    if(F==0)
    {
        SpecShader->use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture5);
        glUniform1i(glGetUniformLocation(shader.ID, "text"), 0);

        float dno=std::min(first[0][1],last[0][1])-0.06;
        float verh=std::max(first[4][1],last[4][1])+0.06;
        float levo=std::min(first[0][0],last[0][0]);
        float pravo=std::max(first[4][0],last[4][0]);

        first[0][0]=levo;
        first[0][1]=dno;

        first[1][0]=pravo;
        first[1][1]=dno;

        first[2][0]=pravo;
        first[2][1]=verh;

        first[3][0]=levo;
        first[3][1]=dno;

        first[4][0]=pravo;
        first[4][1]=verh;

        first[5][0]=levo;
        first[5][1]=verh;

        for(int kk=0;kk<6;kk++)
        {
            if(first[kk][2]==1)
            {
                first[kk][2]=1;
            }
            if(first[kk][3]==1)
            {
                first[kk][3]=10;
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(first), first);

        glDrawArrays(GL_TRIANGLES, 0, 6);

    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void ClientGui::reset() {
    for(int i=0;i<6;i++)
    {
        for(int j=0;j<6;j++)
        {
            for(int q=0;q<6;q++)
            {
                used[i][j][q]=0;
                timedel[i][j][q]=-20;
            }
        }
    }
    del=0;
    mydel=0;
    was_deleted=0;
}
void ClientGui::Init() {
    ror[1][0]=p;
    ror[2][0]=p/2;
    ror[3][0]=3*p/2;
    ror[4][0]=3*p/2;
    ror[5][0]=p/2;


    ClientGui::camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    ClientGui::firstMouse = true;
    ClientGui::lastX = 400, ClientGui::lastY = 300;
    ClientGui::lastX = 0.0f;
    ClientGui::lastY = 0.0f;
    ClientGui::x = 5;
    ClientGui::y = 5;
    ClientGui::z = 5;
    ClientGui::fov = 45.0f;
    ClientGui::deltaTime = 0.0f;
    ClientGui::lastFrame = 0.0f;

    for(int i=0;i<6;i++)
    {
        for(int j=0;j<6;j++)
        {
            for(int q=0;q<6;q++)
            {
                ClientGui::used[i][j][q]=0;
                ClientGui::type[i][j][q]=0; ////
            }
        }
    }



    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    if (!glfwInit())
    {
        std::cout << "[ERR] Failed to init GLFW" << std::endl;
        exit(-1);
    }

    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    ClientGui::screenWidth = mode->width, ClientGui::screenHeight = mode->height;

    std::cout << glfwGetVersionString() << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);


    ClientGui::window = glfwCreateWindow(mode->width, ClientGui::screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
    if (ClientGui::window == nullptr)
        std::cout << "Window is null!!" << std::endl;

    int error;
    const char* description;
    error = glfwGetError(&description);
    if (error != GLFW_NO_ERROR) {
        std::cout << "GLFW Error (" << error << "): " << description << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(ClientGui::window);

    // Set the required callback functions
    glfwSetKeyCallback(ClientGui::window, key_callback);
    glfwSetCursorPosCallback(ClientGui::window, mouse_callback);
    glfwSetScrollCallback(ClientGui::window, scroll_callback);


    // Options

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    int vpwidth, vpheight;
    glfwGetFramebufferSize(ClientGui::window, &vpwidth, &vpheight);
    glViewport(0, 0, vpwidth, vpheight);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Build and compile our shader program
    ClientGui::ourShader = new Shader("../Graphics/v1.txt","../Graphics/f1.txt");
    ClientGui::KnopkiShader = new Shader("../Graphics/v2.txt","../Graphics/f2.txt");
    ClientGui::TShader = new Shader("../Graphics/Vtext.txt","../Graphics/Ftext.txt");
    ClientGui::TextShader = new Shader("../Graphics/vtt.txt","../Graphics/ftt.txt");
    ClientGui::SpecShader = new Shader("../Graphics/vtt.txt","../Graphics/vspec.txt");
    //std::cout<<"textID="<<TextShader->ID<<std::endl;
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat V[]={
            -0.5f, -0.5f, -0.5f,  0.3f, -0.3f,
            0.5f, -0.5f, -0.5f,  0.3f, 0.3f,
            0.5f,  0.5f, -0.5f,  -0.3f, 0.3f,
            0.5f,  0.5f, -0.5f,  -0.3f, 0.3f,
            -0.5f,  0.5f, -0.5f, -0.3f, -0.3f,
            -0.5f, -0.5f, -0.5f,  0.3f, -0.3f,
    };
    GLfloat vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,


            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.35f, -0.35f,
            0.5f, -0.5f, -0.5f,  0.35f, 0.35f,
            0.5f, -0.5f,  0.5f,  -0.35f, 0.35f,
            0.5f, -0.5f,  0.5f,  -0.35f, 0.35f,
            -0.5f, -0.5f,  0.5f,  -0.35f, -0.35f,
            -0.5f, -0.5f, -0.5f,  0.35f, -0.35f,

            -0.5f,  0.5f, -0.5f,  0.35f, -0.35f,
            0.5f,  0.5f, -0.5f,  0.35f, 0.35f,
            0.5f,  0.5f,  0.5f,  -0.35f, 0.35f,
            0.5f,  0.5f,  0.5f,  -0.35f, 0.35f,
            -0.5f,  0.5f,  0.5f,  -0.35f, -0.35f,
            -0.5f,  0.5f, -0.5f,  0.35f, -0.35f
    };

    GLfloat verticesnorm[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f
    };


    glGenVertexArrays(1, &ClientGui::VAO);
    glGenBuffers(1, &ClientGui::VBO);
    glGenVertexArrays(1, &ClientGui::VAO1);
    glGenBuffers(1, &ClientGui::VBO1);
    glGenVertexArrays(1, &ClientGui::VAONORM);
    glGenBuffers(1, &ClientGui::VBONORM);

    glBindVertexArray(ClientGui::VAO);
    glBindBuffer(GL_ARRAY_BUFFER, ClientGui::VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0); // Unbind VAO



    glBindVertexArray(ClientGui::VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, ClientGui::VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(V), V, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0); // Unbind VAO





    glBindVertexArray(ClientGui::VAONORM);
    glBindBuffer(GL_ARRAY_BUFFER, ClientGui::VBONORM);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesnorm), verticesnorm, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0); // Unbind VAO



    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);




    // FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        exit(0);
    }

    // find path to font
    std::string font_name = "../Src/font.ttf";
    if (font_name.empty())
    {
        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        exit(0);
    }

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        exit(0);
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);



    // --== TEXTURE 1 == --
    glGenTextures(1, &ClientGui::texture1);
    glBindTexture(GL_TEXTURE_2D, ClientGui::texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps

    unsigned char* image = SOIL_load_image("../Src/D.jpg", &ClientGui::width, &ClientGui::height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ClientGui::width, ClientGui::height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    // --== TEXTURE 2 == --
    glGenTextures(1, &ClientGui::texture2);
    glBindTexture(GL_TEXTURE_2D, ClientGui::texture2);
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    image = SOIL_load_image("../Src/A.jpg", &ClientGui::width, &ClientGui::height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ClientGui::width, ClientGui::height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);


    // --== TEXTURE 3 == --
    glGenTextures(1, &ClientGui::texture3);
    glBindTexture(GL_TEXTURE_2D, ClientGui::texture3);
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Load, create texture and generate mipmaps
    image = SOIL_load_image("../Src/forest.png", &ClientGui::width, &ClientGui::height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ClientGui::width, ClientGui::height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);


    // --== TEXTURE 4 == --
    glGenTextures(1, &ClientGui::texture4);
    glBindTexture(GL_TEXTURE_2D, ClientGui::texture4);
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Load, create texture and generate mipmaps
    image = SOIL_load_image("../Src/Ha.png", &ClientGui::width, &ClientGui::height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ClientGui::width, ClientGui::height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);


    // --== TEXTURE 5 == --
    glGenTextures(1, &ClientGui::texture5);
    glBindTexture(GL_TEXTURE_2D, ClientGui::texture5);
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Load, create texture and generate mipmaps
    image = SOIL_load_image("../Src/Doroga.png", &ClientGui::width, &ClientGui::height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ClientGui::width, ClientGui::height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    for(int i=0;i<6;i++)
    {
        for(int j=0;j<6;j++)
        {
            for(int q=0;q<6;q++)
            {
                GLfloat a=i,bb=j,c=q;
                ClientGui::kuby.push_back({{(a*1.2f)-3.0f,(bb*1.2f)-3.0f},(c*1.2f)-3.0f});
            }
        }
    }

    ClientGui::knopki.push_back({{-0.04,-0.6},{0.1,0.0015}});
    ClientGui::knopki.push_back({{-0.04,-0.2},{0.3,0.0015}});
    ClientGui::knopki.push_back({{-0.04,-0.4},{0.36,0.0015}});
    ClientGui::knopki.push_back({{-0.04,0},{0.3,0.0015}});

    ClientGui::stroki.push_back("Exit");
    ClientGui::stroki.push_back("Start");
    ClientGui::stroki.push_back("How to play");
    ClientGui::stroki.push_back("Settings");


    ClientGui::strelki.push_back({0.345,-0.2});
    ClientGui::strelki.push_back({0.345,0});
    ClientGui::strelki.push_back({0.345,-0.4});
    for(int i=0;i<ClientGui::strelki.size();i++)
    {
        ClientGui::tigi.push_back(ClientGui::strelki[i]);
        grom.push_back(0.5);
        tigi[i].first-=0.5;
    }
}

bool ClientGui::isGood() {
    return !glfwWindowShouldClose(ClientGui::window);
}
glm::mat4 ClientGui::E()
{
    glm::mat4 A;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            if(i==j)
            {
                A[i][j]=1;
            }
            else
            {
                A[i][j]=0;
            }
        }
    }
    return A;
}
void ClientGui::DrawFrame(std::map<std::pair<std::string, unsigned short>, std::vector<GLuint>>&data,std::map<std::pair<std::string, unsigned short>, std::vector<GLuint>>&data_cubes) {
    //glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_FALSE);

    GLfloat currentFrame = glfwGetTime();
    ClientGui::deltaTime = currentFrame - ClientGui::lastFrame;
    ClientGui::lastFrame = currentFrame;


    glfwPollEvents();
    Do_Movement();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




    //tut bula textura
    for (auto &[key, value]: data_cubes) {
        if (ClientGui::used[value[0]][value[1]][value[2]] != 1 && value[3] < 1000) {
            if(ClientGui::used[value[0]][value[1]][value[2]]!=2)
            {
                ClientGui::used[value[0]][value[1]][value[2]] = 2;
                del=mydel+value[3];
                //data_cubes.erase(key);
            }

        }

        if(value[3] == 2000 && del > 5)
        {
            std::cout<<"konec"<<std::endl;
            ClientGui::del = 20;
        }

        if(value[3] == 1000)
        {
            dx=value[0];
            dx-=100;
            dy=value[1];
            dy-=100;
            dz=value[2];
            dz-=100;
        }

    }
    data_cubes.clear();

    if (F == 0) {


        //ClientGui::TShader->use();
        //ClientGui::TextShader->use();
        std::string v1,v2;
        v1="your points ";
        v2="opponent's points ";
        v1+=std::to_string(mydel);
        v2+=std::to_string(del-mydel);
        RenderText(*ClientGui::TShader, v1,-1,0.8,0.0015, glm::vec3(1, 1, 1),1000);
        RenderText(*ClientGui::TShader, v2,-1,0.6,0.0015, glm::vec3(1, 1, 1),1000);
        glDisable(GL_CULL_FACE);
        ClientGui::ourShader->use();
        glm::mat4 view = E();

        view = ClientGui::camera->GetViewMatrix();
        glm::mat4 projection = E();

        projection = glm::perspective(ClientGui::camera->Zoom,
                                      (float) (ClientGui::screenWidth) / (float) ClientGui::screenHeight, 0.1f,
                                      1000.0f);
        GLint viewLoc = glGetUniformLocation(ClientGui::ourShader->ID, "view");
        GLint projLoc = glGetUniformLocation(ClientGui::ourShader->ID, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(ClientGui::VAO);
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                for (int q = 0; q < 6; q++) {
                    if (ClientGui::used[i][j][q] != 2) {
                        ClientGui::used[i][j][q] = 0;
                    }
                }
            }
        }
        for (auto &[key, value]: data) {
            if (value[3] == 1) {
                //ClientGui::used[value[0]][value[1]][value[2]] = 1;
            }


        }

        //ClientGui::used[ClientGui::x][ClientGui::y][ClientGui::z] = 1;


        glm::mat4 model = E();
        model = glm::scale(model, glm::vec3(50, 50, 50));
        GLint modelLoc = glGetUniformLocation(ClientGui::ourShader->ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(ClientGui::VAONORM);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ClientGui::texture2);
        glUniform1i(glGetUniformLocation(ClientGui::ourShader->ID, "ourTexture1"), 2);
        glUniform3f(glGetUniformLocation(ClientGui::ourShader->ID, "ind"), 1, 1, 1);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = E();
        model = glm::translate(model,glm::vec3(dx,dy,dz));
        model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(ClientGui::VAONORM);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ClientGui::texture4);
        glUniform1i(glGetUniformLocation(ClientGui::ourShader->ID, "ourTexture1"), 2);
        glUniform3f(glGetUniformLocation(ClientGui::ourShader->ID, "ind"), 1, 1, 1);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        for (int i = 0; i < ClientGui::kuby.size(); i++) {
            float c0, c1, c2, c3;
            c0 = ror[type[(i / 36)][((i % 36) / 6)][(i % 6)]][0];
            c1 = ror[type[(i / 36)][((i % 36) / 6)][(i % 6)]][1];
            c2 = ror[type[(i / 36)][((i % 36) / 6)][(i % 6)]][2];
            c3 = ror[type[(i / 36)][((i % 36) / 6)][(i % 6)]][3];
            glm::mat4 model = E();
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
            model = glm::translate(model, glm::vec3(ClientGui::kuby[i].first.first, ClientGui::kuby[i].first.second,
                                                    ClientGui::kuby[i].second));
            GLfloat T = currentFrame - timedel[(i / 36)][((i % 36) / 6)][(i % 6)];
            if (ClientGui::used[(i / 36)][((i % 36) / 6)][(i % 6)] == 2) {
                if (T > 5) {
                    continue;
                } else {
                    int h = type[(i / 36)][((i % 36) / 6)][(i % 6)];
                    model = glm::translate(model,
                                           glm::vec3(ClientGui::move[h][0] * 20 * T, ClientGui::move[h][1] * 20 * T,
                                                     ClientGui::move[h][2] * 20 * T));
                    model = glm::rotate(model, currentFrame * p * 2, glm::vec3(0.0, 0.0, 1.0));
                }
            }

            if (type[(i / 36)][((i % 36) / 6)][(i % 6)] != 0) {
                model = glm::rotate(model, c0, glm::vec3(c1, c2, c3));
            }
            GLint modelLoc = glGetUniformLocation(ClientGui::ourShader->ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            if (ClientGui::used[(i / 36)][((i % 36) / 6)][(i % 6)] == 1) {
                glBindVertexArray(ClientGui::VAONORM);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, ClientGui::texture2);
                glUniform1i(glGetUniformLocation(ClientGui::ourShader->ID, "ourTexture1"), 1);
            } else {
                glBindVertexArray(ClientGui::VAO);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, ClientGui::texture1);
                glUniform1i(glGetUniformLocation(ClientGui::ourShader->ID, "ourTexture1"), 0);
            }
            if(used[(i / 36)][((i % 36) / 6)][(i % 6)]==0 && T<0.3)
            {
                glUniform3f(glGetUniformLocation(ClientGui::ourShader->ID, "ind"), 1, 1, 0.9);
            }
            else
            {
                glUniform3f(glGetUniformLocation(ClientGui::ourShader->ID, "ind"), 1, 1, 1);
            }

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glFlush();
        glFinish();
        glBindVertexArray(0);


        glfwSwapBuffers(ClientGui::window);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ClientGui::end = std::chrono::steady_clock::now();
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && ClientGui::flag == 0 && ClientGui::F==0 && std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() >400000) {
            ClientGui::begin = ClientGui::end;
            ClientGui::flag = 1;
            glBindVertexArray(ClientGui::VAO);
            for (int i = 0; i < ClientGui::kuby.size(); i++) {
                glm::mat4 model = E();

                model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
                model = glm::translate(model, glm::vec3(ClientGui::kuby[i].first.first, ClientGui::kuby[i].first.second,
                                                        ClientGui::kuby[i].second));

                GLint modelLoc = glGetUniformLocation(ClientGui::ourShader->ID, "model");
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

                GLint j1 = i / 36, j2 = (i % 36) / 6, j3 = i % 6;
                GLfloat i1 = j1, i2 = j2, i3 = j3;
                i1 /= 10;
                i2 /= 10;
                i3 /= 10;
                glUniform3f(glGetUniformLocation(ClientGui::ourShader->ID, "ind"), i1, i2, i3);
                if (ClientGui::used[(i / 36)][((i % 36) / 6)][(i % 6)] < 2) {
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }

            glFlush();
            glFinish();
            glBindVertexArray(0);
            GLint X = ClientGui::screenWidth / 2, Y = ClientGui::screenHeight / 2;

            GLfloat *R, *B, *G;

            GLfloat data[4];
            glReadPixels(X, Y, 1, 1, GL_RGBA, GL_FLOAT, data);


            ClientGui::r = round(data[0] * 10);
            ClientGui::g = round(data[1] * 10);
            ClientGui::b = round(data[2] * 10);

            if (ClientGui::r == ClientGui::b && ClientGui::b == ClientGui::g && ClientGui::b == 10) {
                //std::cout<<"BEL"<<std::endl;
            } else {
                //std::cout<<*R<<" "<<*G<<" "<<*B<<" "<<std::endl;
                //std::cout<<r<<" "<<g<<" "<<b<<" "<<std::endl;
                ClientGui::timedel[ClientGui::r][ClientGui::g][ClientGui::b] = currentFrame;
                if (ClientGui::used[ClientGui::r][ClientGui::g][ClientGui::b] < 1) {
                    int r1 = r, g1 = g, b1 = b, kot = 0;
                    //std::cout<<"type "<<ClientGui::type[ClientGui::r][ClientGui::g][ClientGui::b]<<" "<<ClientGui::move[ClientGui::type[ClientGui::r][ClientGui::g][ClientGui::b]][0]<<" "<<ClientGui::move[ClientGui::type[ClientGui::r][ClientGui::g][ClientGui::b]][1]<<" "<<ClientGui::move[ClientGui::type[ClientGui::r][ClientGui::g][ClientGui::b]][2]<<std::endl;
                    r1 += ClientGui::move[ClientGui::type[ClientGui::r][ClientGui::g][ClientGui::b]][0];
                    g1 += ClientGui::move[ClientGui::type[ClientGui::r][ClientGui::g][ClientGui::b]][1];
                    b1 += ClientGui::move[ClientGui::type[ClientGui::r][ClientGui::g][ClientGui::b]][2];
                    while (r1 >= 0 && g1 >= 0 && b1 >= 0 && r1 < 6 && g1 < 6 && b1 < 6) {
                        //std::cout<<r1<<" "<<g1<<" "<<b1<<" "<<ClientGui::used[r][g][b]<<std::endl;
                        if (ClientGui::used[r1][g1][b1] < 2) {
                            kot = 1;
                            break;
                        }
                        r1 += ClientGui::move[ClientGui::type[ClientGui::r][ClientGui::g][ClientGui::b]][0];
                        g1 += ClientGui::move[ClientGui::type[ClientGui::r][ClientGui::g][ClientGui::b]][1];
                        b1 += ClientGui::move[ClientGui::type[ClientGui::r][ClientGui::g][ClientGui::b]][2];
                    }
                    if (kot == 0) {
                        ClientGui::used[ClientGui::r][ClientGui::g][ClientGui::b] = 2;
                        ClientGui::was_deleted = 1;
                        del++;
                        mydel++;
                    } else{
                        ClientGui::wrong = true;
                    }

                    //std::cout<<"END"<<std::endl;
                }

            }

        }


        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && flag == 1 && ClientGui::F==0) {
            ClientGui::flag = 0;
        }

        //SEND(camera->Position.x,camera->Position.y,camera->Position.z,10);
    }
    else
    {
        if(ClientGui::in_game)
        {
            ClientGui::stroki[1]="continue";
        }
        else
        {
            ClientGui::stroki[1]="Start";
        }
        glEnable(GL_CULL_FACE);

        ClientGui::KnopkiShader->use();

        glBindVertexArray(ClientGui::VAONORM);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ClientGui::texture2);
        glUniform1i(glGetUniformLocation(ClientGui::KnopkiShader->ID, "ourTexture1"), 0);


        glm::mat4 model = E();
        model = glm::scale(model, glm::vec3(2, 2, 1));
        model = glm::translate(model, glm::vec3(0, 0, 1.1));

        GLint modelLoc = glGetUniformLocation(ClientGui::KnopkiShader->ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glUniform3f(glGetUniformLocation(ClientGui::KnopkiShader->ID, "ind"), 1, 0.9, 1);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        //std::cout<<"F="<<F<<" flag="<<flag<<std::endl;
            if(ClientGui::F==1)
            {
                glBindVertexArray(ClientGui::VAO1);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, ClientGui::texture1);

                for (int i = 0; i < ClientGui::knopki.size(); i++) {
                    //std::cout<<"i="<<i<<std::endl;
                    glm::mat4 model = E();
                    model = glm::translate(model, glm::vec3(ClientGui::knopki[i].first.first,
                                                            ClientGui::knopki[i].first.second,
                                                            1));
                    model = glm::scale(model,
                                       glm::vec3(1.4, 0.1,
                                                 1));
                    GLint modelLoc = glGetUniformLocation(ClientGui::KnopkiShader->ID, "model");
                    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                    if(i==1)
                    {
                        glUniform3f(glGetUniformLocation(ClientGui::KnopkiShader->ID, "ind"), 0.34, 0.63, 0.68);
                    }
                    else
                    {
                        glUniform3f(glGetUniformLocation(ClientGui::KnopkiShader->ID, "ind"), 0.48, 0.45, 0.42);
                    }

                    glDrawArrays(GL_TRIANGLES, 0, 6);

                }
                glFlush();
                glFinish();
                glBindVertexArray(ClientGui::VAONORM);
                ClientGui::TextShader->use();
                for (int i = 0; i < ClientGui::knopki.size(); i++) {
                    RenderText(*ClientGui::TextShader, ClientGui::stroki[i],
                               ClientGui::knopki[i].first.first-0.3 ,
                               ClientGui::knopki[i].first.second-0.025,
                               ClientGui::knopki[i].second.second, glm::vec3(30.85, 0.89f, 0.82f),100);
                }


                glFlush();
                glFinish();
                glfwSwapBuffers(ClientGui::window);

                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && ClientGui::flag == 0 && ClientGui::F==1) {
                    glBindVertexArray(ClientGui::VAONORM);
                    ClientGui::flag = 1;
                    ClientGui::KnopkiShader->use();
                    for (int i = 0; i < ClientGui::knopki.size(); i++) {
                        GLint j1 = i / 36, j2 = (i % 36) / 6, j3 = i % 6;
                        GLfloat i1 = j1, i2 = j2, i3 = j3;
                        i1 /= 10;
                        i2 /= 10;
                        i3 /= 10;

                        glUniform3f(glGetUniformLocation(ClientGui::KnopkiShader->ID, "ind"), i1, i2, i3);

                        glm::mat4 model = E();
                        model = glm::translate(model, glm::vec3(ClientGui::knopki[i].first.first,
                                                                ClientGui::knopki[i].first.second,
                                                                1));
                        model = glm::scale(model,
                                           glm::vec3(1.4, 0.1,
                                                     1));
                        GLint modelLoc = glGetUniformLocation(ClientGui::KnopkiShader->ID, "model");
                        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

                        glDrawArrays(GL_TRIANGLES, 0, 6);


                    }
                    glFlush();
                    glFinish();

                    double X, Y;
                    glfwGetCursorPos(window, &X, &Y);

                    Y = screenHeight - Y;
                    GLfloat data[4];
                    glReadPixels(X, Y, 1, 1, GL_RGBA, GL_FLOAT, data);


                    ClientGui::r = round(data[0] * 10);
                    ClientGui::g = round(data[1] * 10);
                    ClientGui::b = round(data[2] * 10);
                    int kys = r * 36 + g * 6 + b;
                    if (ClientGui::r == ClientGui::b && ClientGui::b == ClientGui::g && ClientGui::b == 10) {
                        //std::cout << "BEL" << std::endl;
                    } else {
                        if (kys == 0) {
                            glfwSetWindowShouldClose(ClientGui::window, GL_TRUE);
                           // exit(0);
                        }

                        if (kys == 1) {
                            //std::cout<<"how??"<<std::endl;
                            ClientGui::F = 3;
                        }

                        if (kys == 2) {
                            ClientGui::F = 2;
                        }

                        if(kys==3)
                        {
                            ClientGui::F = 7;
                        }
                    }


                }

                glFlush();
                glFinish();

                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && ClientGui::flag == 1 && ClientGui::F==1) {
                    ClientGui::flag = 0;
                }
            }
            else
            {
                //std::cout<<"AAAAAAAAAA"<<std::endl;
                if(ClientGui::F==2)
                {

                    ClientGui::TextShader->use();
                    std::string s1="By looking at the sides of the cube, you can understand which way it is directed (up, down, right, left, away from yourself, towards yourself). You can click on the cube and it will fly away in the direction in which it is directed, but only if there is no other cube in its path";
                    std::string s2="This is a multiplayer game, you will get a random opponent and the one who can remove more cubes wins";
                    RenderText(*ClientGui::TextShader, s1,-0.95,0.8,0.0015, glm::vec3(0.33, 0.32f, 0.30f),1000);
                    RenderText(*ClientGui::TextShader, s2,-0.95,-0.4,0.0015, glm::vec3(0.33, 0.32f, 0.30f),1000);
                    glFlush();
                    glFinish();
                    glfwSwapBuffers(ClientGui::window);
                }
                else
                {
                    if(ClientGui::F==3)
                    {
                        ClientGui::TextShader->use();
                        std::string s1="waiting...";
                        RenderText(*ClientGui::TextShader, s1,-0.4,-0.1,0.005, glm::vec3(0.33, 0.32f, 0.30f),1000);

                        glFlush();
                        glFinish();
                        glfwSwapBuffers(ClientGui::window);

                        if(ClientGui::in_game){
                            ClientGui::F = 0;
                        }
                    }
                    else
                    {
                        if(ClientGui::F==4)
                        {
                            ClientGui::TextShader->use();
                            std::string s1="WIN";
                            RenderText(*ClientGui::TextShader, s1,-0.3,-0.1,0.005, glm::vec3(1.0, 1.0f, 1.0f),1000);

                            glFlush();
                            glFinish();
                            glfwSwapBuffers(ClientGui::window);
                        }
                        else
                        {
                            if(ClientGui::F==5)
                            {
                                ClientGui::TextShader->use();
                                std::string s1="WIN";
                                RenderText(*ClientGui::TextShader, s1,-0.3,-0.1,0.005, glm::vec3(1.0, 1.0f, 1.0f),1000);

                                glFlush();
                                glFinish();
                                glfwSwapBuffers(ClientGui::window);
                            }
                            else
                            {
                                if(ClientGui::F==6)
                                {
                                    ClientGui::TextShader->use();
                                    std::string s1="LOSE";
                                    RenderText(*ClientGui::TextShader, s1,-0.35,-0.1,0.005, glm::vec3(1.0, 1.0f, 1.0f),1000);

                                    glFlush();
                                    glFinish();
                                    glfwSwapBuffers(ClientGui::window);
                                }
                                else
                                {
                                    if(ClientGui::F==7)
                                    {
                                        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                                        ClientGui::KnopkiShader->use();

                                        glBindVertexArray(ClientGui::VAONORM);
                                        glActiveTexture(GL_TEXTURE0);
                                        glBindTexture(GL_TEXTURE_2D, ClientGui::texture2);
                                        glUniform1i(glGetUniformLocation(ClientGui::KnopkiShader->ID, "ourTexture1"), 0);


                                        glm::mat4 model = E();
                                        model = glm::scale(model, glm::vec3(2, 2, 1));
                                        model = glm::translate(model, glm::vec3(0, 0, 1.1));

                                        GLint modelLoc = glGetUniformLocation(ClientGui::KnopkiShader->ID, "model");
                                        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                                        glUniform3f(glGetUniformLocation(ClientGui::KnopkiShader->ID, "ind"), 1, 1, 1);
                                        glDrawArrays(GL_TRIANGLES, 0, 6);




                                        ClientGui::KnopkiShader->use();

                                        glBindVertexArray(ClientGui::VAO1);
                                        glActiveTexture(GL_TEXTURE0);
                                        glBindTexture(GL_TEXTURE_2D, ClientGui::texture1);
                                        glUniform1i(glGetUniformLocation(ClientGui::KnopkiShader->ID, "ourTexture1"), 0);
                                        glUniform3f(glGetUniformLocation(ClientGui::KnopkiShader->ID, "ind"), 1, 1, 1);
                                        for(int i=0;i<strelki.size();i++)
                                        {
                                            glm::mat4 model = E();
                                            model = glm::translate(model, glm::vec3(strelki[i].first, strelki[i].second, 1));
                                            model = glm::scale(model, glm::vec3(0.69, 0.1, 1));


                                            GLint modelLoc = glGetUniformLocation(ClientGui::KnopkiShader->ID, "model");
                                            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

                                            glDrawArrays(GL_TRIANGLES, 0, 6);
                                        }

                                        glBindVertexArray(ClientGui::VAONORM);
                                        glActiveTexture(GL_TEXTURE0);
                                        glBindTexture(GL_TEXTURE_2D, ClientGui::texture2);
                                        glUniform1i(glGetUniformLocation(ClientGui::KnopkiShader->ID, "ourTexture1"), 0);

                                        for(int i=0;i<tigi.size();i++)
                                        {
                                            glm::mat4 model = E();
                                            model = glm::translate(model, glm::vec3(tigi[i].first+grom[i], tigi[i].second, 0.9));
                                            model = glm::scale(model, glm::vec3(0.1, 0.13, 1));


                                            GLint modelLoc = glGetUniformLocation(ClientGui::KnopkiShader->ID, "model");
                                            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                                            glUniform3f(glGetUniformLocation(ClientGui::KnopkiShader->ID, "ind"),0.48, 0.45, 0.42);
                                            glDrawArrays(GL_TRIANGLES, 0, 6);
                                        }


                                        glActiveTexture(GL_TEXTURE0);
                                        glBindTexture(GL_TEXTURE_2D, ClientGui::texture3);
                                        glUniform1i(glGetUniformLocation(ClientGui::KnopkiShader->ID, "ourTexture1"), 0);

                                        for(int i=0;i<strelki.size();i++)
                                        {
                                            glm::mat4 model = E();
                                            model = glm::translate(model, glm::vec3(strelki[i].first-0.395, strelki[i].second, 0.9));
                                            model = glm::scale(model, glm::vec3(0.05, 0.1, 1));


                                            GLint modelLoc = glGetUniformLocation(ClientGui::KnopkiShader->ID, "model");
                                            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                                            glUniform3f(glGetUniformLocation(ClientGui::KnopkiShader->ID, "ind"), 0.48, 0.45, 0.42);
                                            glDrawArrays(GL_TRIANGLES, 0, 6);



                                            model = E();
                                            model = glm::translate(model, glm::vec3(strelki[i].first+0.395, strelki[i].second, 0.9));
                                            model = glm::scale(model, glm::vec3(0.05, 0.1, 1));


                                            modelLoc = glGetUniformLocation(ClientGui::KnopkiShader->ID, "model");
                                            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                                            glUniform3f(glGetUniformLocation(ClientGui::KnopkiShader->ID, "ind"), 0.48, 0.45, 0.42);
                                            glDrawArrays(GL_TRIANGLES, 0, 6);
                                        }

                                        ClientGui::TextShader->use();
                                        RenderText(*ClientGui::TextShader, "Music Volume",-0.85,-0.025,0.0015, glm::vec3(0.33, 0.32f, 0.30f),1000);
                                        RenderText(*ClientGui::TextShader, "Game Volume",-0.85,-0.2-0.025,0.0015, glm::vec3(0.33, 0.32f, 0.30f),1000);
                                        RenderText(*ClientGui::TextShader, "Mouse sensitivity",-0.85,-0.4-0.025,0.0015, glm::vec3(0.33, 0.32f, 0.30f),1000);
                                        for(int i=0;i<strelki.size();i++)
                                        {
                                            RenderText(*ClientGui::TextShader, "-",strelki[i].first-0.395-0.0125,strelki[i].second-0.025,0.0015, glm::vec3(0.85, 0.89f, 0.82f),1000);
                                            RenderText(*ClientGui::TextShader, "+",strelki[i].first+0.395-0.02,strelki[i].second-0.025,0.0015, glm::vec3(0.85, 0.89f, 0.82f),1000);
                                        }


                                        ClientGui::KnopkiShader->use();
                                        glFlush();
                                        glFinish();
                                        glfwSwapBuffers(ClientGui::window);

                                        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                                        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && ClientGui::flag == 0 && ClientGui::F==7)
                                        {
                                            glBindVertexArray(ClientGui::VAONORM);
                                            ClientGui::flag = 1;
                                            ClientGui::KnopkiShader->use();
                                            for (int i = 0; i < ClientGui::strelki.size(); i++) {
                                                GLfloat j = i;
                                                GLfloat i1 = j / 10, i2 = 0, i3 = 0;
                                                glUniform3f(glGetUniformLocation(ClientGui::KnopkiShader->ID, "ind"),
                                                            i1, i2, i3);

                                                glm::mat4 model = E();
                                                model = glm::translate(model, glm::vec3(strelki[i].first-0.395, strelki[i].second, 0.9));
                                                model = glm::scale(model, glm::vec3(0.05, 0.1, 1));


                                                GLint modelLoc = glGetUniformLocation(ClientGui::KnopkiShader->ID,
                                                                                      "model");
                                                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                                                glDrawArrays(GL_TRIANGLES, 0, 6);


                                                i2 = 1;
                                                glUniform3f(glGetUniformLocation(ClientGui::KnopkiShader->ID, "ind"),
                                                            i1, i2, i3);
                                                model = E();
                                                model = glm::translate(model, glm::vec3(strelki[i].first+0.395, strelki[i].second, 0.9));
                                                model = glm::scale(model, glm::vec3(0.05, 0.1, 1));


                                                modelLoc = glGetUniformLocation(ClientGui::KnopkiShader->ID, "model");
                                                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                                                glDrawArrays(GL_TRIANGLES, 0, 6);

                                            }


                                            glFlush();
                                            glFinish();

                                            double X, Y;
                                            glfwGetCursorPos(window, &X, &Y);

                                            Y = screenHeight - Y;
                                            GLfloat data[4];
                                            glReadPixels(X, Y, 1, 1, GL_RGBA, GL_FLOAT, data);


                                            ClientGui::r = round(data[0] * 10);
                                            ClientGui::g = round(data[1] * 10);
                                            ClientGui::b = round(data[2] * 10);

                                            //std::cout << "r=" << r << " g=" << g << std::endl;
                                            if (ClientGui::r == ClientGui::b && ClientGui::b == ClientGui::g &&
                                                ClientGui::b == 10) {
                                                //std::cout << "BEL" << std::endl;
                                            } else {
                                                if (g == 0) {
                                                    if (grom[r] > 0.2) {
                                                        grom[r] -= 0.05;
                                                        if(r == 0){
                                                            gamesound -= 0.5f/6;
                                                        } else if(r == 1){
                                                            musicsound -= 0.5f/6;
                                                        }

                                                    }
                                                } else {
                                                    if (grom[r] < 0.8) {
                                                        grom[r] += 0.05;
                                                        if(r == 0){
                                                            gamesound += 0.5f/6;
                                                        } else if(r == 1){
                                                            musicsound += 0.5f/6;
                                                        }
                                                    }
                                                }

                                            }



                                        }

                                        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && ClientGui::flag == 1 && ClientGui::F==7)
                                        {
                                            ClientGui::flag = 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

            }
    }

}

void ClientGui::Finish(){
    delete ClientGui::camera;
    glDeleteVertexArrays(1, &ClientGui::VAO);
    glDeleteBuffers(1, &ClientGui::VBO);
    glDeleteVertexArrays(1, &ClientGui::VAONORM);
    glDeleteBuffers(1, &ClientGui::VBONORM);
    glDeleteVertexArrays(1, &ClientGui::vao);
    glDeleteBuffers(1, &ClientGui::vbo);
    glfwTerminate();
}

ClientGui::~ClientGui()
{
}


void ClientGui::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(F!=1)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { ////
            if(ClientGui::F == 3)ClientGui::esc_waiting = true;
            ClientGui::F=1;
        }
    }

    if(ClientGui::F==0) {
        //cout << key << endl;
        if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
            if (ClientGui::y < 5) {
                ClientGui::y++;
            }
        }

        if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
            if (ClientGui::y > 0) {
                ClientGui::y--;
            }
        }

        if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
            if (ClientGui::x > 0) {
                ClientGui::x--;
            }
        }

        if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
            if (ClientGui::x < 5) {
                ClientGui::x++;
            }
        }

        if (key == GLFW_KEY_U && action == GLFW_PRESS) {
            if (ClientGui::z < 5) {
                ClientGui::z++;
            }
        }

        if (key == GLFW_KEY_J && action == GLFW_PRESS) {
            if (ClientGui::z > 0) {
                ClientGui::z--;
            }
        }

        if (key >= 0 && key < 1024) {
            if (action == GLFW_PRESS)
                ClientGui::keys[key] = true;
            else if (action == GLFW_RELEASE)
                ClientGui::keys[key] = false;
        }
    }

}

void ClientGui::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(ClientGui::fov >= 1.0f && ClientGui::fov <= 45.0f)
        ClientGui::fov -= (yoffset/10);
    if(ClientGui::fov <= 1.0f)
        ClientGui::fov = 1.0f;
    if(ClientGui::fov >= 45.0f)
        ClientGui::fov = 45.0f;
}
//poka ne rabotaet

void ClientGui::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //if(keys[GLFW_KEY_SPACE]== false)
    //{
    if(F==0)
    {
        glfwSetCursorPos(window,ClientGui::screenWidth/2,ClientGui::screenHeight/2);
        //std::cout<<screenWidth<<" "<<screenHeight<<" "<<screenWidth/2<<" "<<screenHeight/2<<std::endl;
        if(ClientGui::firstMouse==true)
        {
            //std::cout<<"AAAAAAAAAAAAa"<<std::endl;
            ClientGui::firstMouse = false;
        }
        else
        {
            GLfloat xoffset = xpos - ClientGui::screenWidth/2;
            GLfloat yoffset = ClientGui::screenHeight/2 - ypos;
            camera->ProcessMouseMovement(xoffset*grom[2], yoffset*grom[2]);
        }
    }


   // }
   // else
    //{
        //ClientGui::firstMouse=true;
   // }
}

void ClientGui::Do_Movement()
{
    // Camera controls
    if(ClientGui::keys[GLFW_KEY_W])
        ClientGui::camera->ProcessKeyboard(FORWARD, ClientGui::deltaTime);
    if(ClientGui::keys[GLFW_KEY_S])
        ClientGui::camera->ProcessKeyboard(BACKWARD, ClientGui::deltaTime);
    if(ClientGui::keys[GLFW_KEY_A])
        ClientGui::camera->ProcessKeyboard(LEFT, ClientGui::deltaTime);
    if(ClientGui::keys[GLFW_KEY_D])
        ClientGui::camera->ProcessKeyboard(RIGHT, ClientGui::deltaTime);
    if(ClientGui::keys[GLFW_KEY_SPACE])
        ClientGui::camera->ProcessKeyboard(SPACE, ClientGui::deltaTime);
}
