#include <iostream>
#include "ClientGui.h"

GLuint ClientGui::x = 5;
GLuint ClientGui::y = 5;
GLuint ClientGui::z = 5;
GLfloat ClientGui::fov = 45.0f;
GLfloat ClientGui::lastX = 0.0f;
GLfloat ClientGui::lastY = 0.0f;
bool ClientGui::firstMouse = false;
bool ClientGui::keys[1024] = {};
Camera* ClientGui::camera = nullptr;
GLFWwindow* ClientGui::window = nullptr;
GLfloat ClientGui::deltaTime = {};
GLfloat ClientGui::lastFrame = {};
GLuint ClientGui::VAO = {};
GLuint ClientGui::VBO = {};
int ClientGui::width = {};
int ClientGui::height = {};
GLuint ClientGui::screenHeight = {};
GLuint ClientGui::screenWidth = {};
std::vector< std::pair< std::pair<GLfloat,GLfloat>,GLfloat > > ClientGui::kuby = {};
Shader* ClientGui::ourShader = nullptr;
GLuint ClientGui::texture1 = {};
GLuint ClientGui::texture2 = {};
int ClientGui::used[6][6][6] = {{0,0,0},{0,0,0},{0,0,0}};


ClientGui::ClientGui() {

}
void ClientGui::Init() {

    ClientGui::camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    ClientGui::firstMouse = true;
    ClientGui::lastX = 400, ClientGui::lastY = 300;
    ClientGui::lastX = 0.0f;
    ClientGui::lastY = 0.0f;
    ClientGui::firstMouse = false;
    ClientGui::x = 5;
    ClientGui::y = 5;
    ClientGui::z = 5;
    ClientGui::fov = 45.0f;
    ClientGui::deltaTime = 0.0f;
    ClientGui::lastFrame = 0.0f;

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

    ClientGui::window = glfwCreateWindow(ClientGui::screenWidth, ClientGui::screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    int vpwidth, vpheight;
    glfwGetFramebufferSize(ClientGui::window, &vpwidth, &vpheight);
    glViewport(0, 0, vpwidth, vpheight);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);



    // Build and compile our shader program
    ClientGui::ourShader = new Shader("../Graphics/v1.txt","../Graphics/f1.txt");


    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {

            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

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

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &ClientGui::VAO);
    glGenBuffers(1, &ClientGui::VBO);

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

    unsigned char* image = SOIL_load_image("../Src/W.jpg", &ClientGui::width, &ClientGui::height, 0, SOIL_LOAD_RGB);
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
    image = SOIL_load_image("../Src/awesomeface.png", &ClientGui::width, &ClientGui::height, 0, SOIL_LOAD_RGB);
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
                GLfloat a=i,b=j,c=q;
                ClientGui::kuby.push_back({{(a*1.2f)-3.0f,(b*1.2f)-3.0f},(c*1.2f)-3.0f});
            }
        }
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
void ClientGui::DrawFrame(std::map<std::pair<std::string, unsigned short>, std::vector<GLuint>>&data)
{
//////////
    GLfloat currentFrame = glfwGetTime();
    ClientGui::deltaTime = currentFrame - ClientGui::lastFrame;
    ClientGui::lastFrame = currentFrame;

    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents();
    Do_Movement();


    // Render
    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




    //tut bula textura

    ClientGui::ourShader->use();

    glm::mat4 view=E();
    view = ClientGui::camera->GetViewMatrix();
    glm::mat4 projection=E();

    projection = glm::perspective(ClientGui::camera->Zoom, (float)ClientGui::screenWidth/(float)ClientGui::screenHeight, 0.1f, 1000.0f);
    GLint viewLoc = glGetUniformLocation(ClientGui::ourShader->ID, "view");
    GLint projLoc = glGetUniformLocation(ClientGui::ourShader->ID, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(ClientGui::VAO);
    for(int i=0;i<6;i++)
    {
        for(int j=0;j<6;j++)
        {
            for(int q=0;q<6;q++)
            {
                ClientGui::used[i][j][q]=0;
            }
        }
    }
    for (auto& [key, value] : data)
    {
        ClientGui::used[value[0]][value[1]][value[2]]=1;
    }
    ClientGui::used[ClientGui::x][ClientGui::y][ClientGui::z]=1;
    for(int i=0;i<ClientGui::kuby.size();i++)
    {
        glm::mat4 model=E();

        model=glm::scale(model,glm::vec3(0.5,0.5,0.5));
        model = glm::translate(model, glm::vec3(ClientGui::kuby[i].first.first, ClientGui::kuby[i].first.second, ClientGui::kuby[i].second));
        //model = glm::rotate(model, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.5f, 1.0f, 0.0f));

        GLint modelLoc = glGetUniformLocation(ClientGui::ourShader->ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        if(ClientGui::used[(i/36)][((i%36)/6)][(i%6)]==0)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ClientGui::texture1);
            glUniform1i(glGetUniformLocation(ClientGui::ourShader->ID, "ourTexture1"), 0);
        }
        else
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ClientGui::texture2);
            glUniform1i(glGetUniformLocation(ClientGui::ourShader->ID, "ourTexture1"), 1);
        }


        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(0);


    glfwSwapBuffers(ClientGui::window);
}

void ClientGui::Finish(){
    delete ClientGui::camera;
    ///
    glDeleteVertexArrays(1, &ClientGui::VAO);
    glDeleteBuffers(1, &ClientGui::VBO);

    glfwTerminate();
}

ClientGui::~ClientGui()
{
}


void ClientGui::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //cout << key << endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(ClientGui::window, GL_TRUE);
        exit(0);
    }


    if(key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        if(ClientGui::y<5)
        {
            ClientGui::y++;
        }
    }

    if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        if(ClientGui::y>0)
        {
            ClientGui::y--;
        }
    }

    if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        if(ClientGui::x>0)
        {
            ClientGui::x--;
        }
    }

    if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        if(ClientGui::x<5)
        {
            ClientGui::x++;
        }
    }

    if(key == GLFW_KEY_U && action == GLFW_PRESS)
    {
        if(ClientGui::z<5)
        {
            ClientGui::z++;
        }
    }

    if(key == GLFW_KEY_J && action == GLFW_PRESS)
    {
        if(ClientGui::z>0)
        {
            ClientGui::z--;
        }
    }

    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            ClientGui::keys[key] = true;
        else if(action == GLFW_RELEASE)
            ClientGui::keys[key] = false;
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
    if(ClientGui::firstMouse)
    {
        /*ClientGui::lastX = xpos;
        ClientGui::lastY = ypos;
        ClientGui::firstMouse = false;*/
        glfwSetCursorPos(window,800,500);
        firstMouse = false;
    }

    GLfloat xoffset = xpos - 800;//ClientGui::lastX
    GLfloat yoffset = 500 - ypos;  //ClientGui::lastY Reversed since ClientGui::y-coordinates go from bottom to left

    //ClientGui::lastX = xpos;
    //ClientGui::lastY = ypos;


    if(keys[GLFW_KEY_SPACE]== false)
    {
        glfwSetCursorPos(window,800,500);
        camera->ProcessMouseMovement(xoffset, yoffset);
    }

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
}
