#include <iostream>
#include <chrono>
#include <thread>
#include <future>
#include <unistd.h>
#include <vector>

//#include <pqxx/pqxx>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "../Messages/msg_export.h"
#include "../Networking/Agent.h"
#include "../Logic/ClientLogic.h"
#include "../Gui/ClientGui.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Camera.h"










// The MAIN function, from here we start the application and run the game loop
//int main()
//{



    // Game loop



    //ty perenes texturu
   // while (!glfwWindowShouldClose(window))
    //{

   // }


   // return 0;
//}



///////






int main()
{
    ClientLogic client;
    ClientGui::Init();

    std::map<std::pair<std::string, unsigned short>, std::vector<GLuint>> data = {};
    std::map<std::pair<std::string, unsigned short>, std::vector<GLuint>> data_cubes = {};
    std::cout << "[LOG] Starting message dispatch loop.." << std::endl;

    const std::chrono::duration<float> targetFrameTime(std::chrono::duration<float>(1.0f / CLIENT_FPS));
    auto lastFrameTime = std::chrono::high_resolution_clock::now();


    while (ClientGui::isGood() && client.isGood())
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = currentTime - lastFrameTime;
        if (elapsedTime < targetFrameTime)
        {
            std::this_thread::sleep_for(targetFrameTime - elapsedTime);
            currentTime = std::chrono::high_resolution_clock::now();
            elapsedTime = currentTime - lastFrameTime;
        }
        ClientGui::was_deleted = 0;
        client.DispatchMessage(data,data_cubes, ClientGui::x, ClientGui::y, ClientGui::z,1);
        ClientGui::DrawFrame(data,data_cubes);
        if(ClientGui::was_deleted){
            //send
           // std::cout<<"lalal"<<std::endl;
            client.DispatchMessage(data,data_cubes, ClientGui::r, ClientGui::g, ClientGui::b,2);
        }
        lastFrameTime = currentTime;

    }

    std::cout << "[LOG] exiting" << std::endl;
    ClientGui::Finish();
    return 0;
}