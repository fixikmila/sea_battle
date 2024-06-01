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

#include <irrKlang.h>
using namespace irrklang;








int main()
{
    ISoundEngine *SoundEngine = createIrrKlangDevice();
    irrklang::ISound* snd = SoundEngine->play2D("../Src/music.mp3", true, false, true), *onesound;

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

        ClientGui::in_game = client.in_game;
        //if(ClientGui::F != 3)std::cout<<"FF"<<ClientGui::F<<std::endl;
        ClientGui::DrawFrame(data,data_cubes);
        if (snd)
            snd->setVolume(ClientGui::musicsound);
        if(ClientGui::was_deleted){
            onesound = SoundEngine->play2D("../Src/bleep.mp3",false,false,true);
            onesound->setVolume(ClientGui::gamesound);
            //send
           // std::cout<<"lalal"<<std::endl;
            client.DispatchMessage(data,data_cubes, ClientGui::r, ClientGui::g, ClientGui::b,2);
        }
        if(ClientGui::wrong){
            ClientGui::wrong = false;
            onesound = SoundEngine->play2D("../Src/erro.mp3",false,false,true);
            onesound->setVolume(ClientGui::gamesound);
        }
        lastFrameTime = currentTime;

        if(ClientGui::del!=0)std::cout<<"del"<<ClientGui::del<<std::endl;
        if(ClientGui::esc_waiting){
            ClientGui::esc_waiting = false;
            client.game_end = true;
            ClientGui::reset();
            //client.isSrvFound = false;
            client.in_game = false;
            data_cubes.clear();
        }
        if(client.in_game && ClientGui::del == client.how_many){//game ended

            std::cout<<"game ended"<<ClientGui::mydel<<" "<<ClientGui::del<<std::endl;
            if(2 * ClientGui::mydel > ClientGui::del){
                std::cout<<"you won"<<std::endl;
                ClientGui::F = 4;
            } else if(ClientGui::mydel * 2== ClientGui::del){
                std::cout<<"draw"<<std::endl;
                ClientGui::F = 5;
            } else{
                std::cout<<"you lost"<<std::endl;
                ClientGui::F = 6;
            }
            ClientGui::reset();
            //client.isSrvFound = false;
            client.in_game = false;
            data_cubes.clear();
            client.game_end = true;
        }
    }

    if (snd)
    {
        snd->drop();
        snd = nullptr;
    }
    if (onesound)
    {
        onesound->drop();
        onesound = nullptr;
    }
    SoundEngine->drop();
    client.game_end = true;
    client.DispatchMessage(data,data_cubes, ClientGui::x, ClientGui::y, ClientGui::z,1);
    std::cout << "[LOG] exiting" << std::endl;
    ClientGui::Finish();
    std::cout<<"END"<<std::endl;
    exit(0);
    return 0;
}