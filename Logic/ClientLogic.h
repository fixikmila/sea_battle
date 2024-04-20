//
// Created by milana on 15.04.24.
//

#ifndef SHIPS_CLIENTLOGIC_H
#define SHIPS_CLIENTLOGIC_H
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include <string>

#include "../Utils/Config.h"
#include "../Messages/msg.h"
#include "../Networking/Agent.h"

class ClientLogic {
private:
    Agent *agent;
    std::pair<std::string, unsigned short> srvaddr;
    bool isSrvFound = false;
public:
    ClientLogic();
    bool isGood();
    void DispatchMessage(std::map<std::pair<std::string, unsigned short>, std::vector<GLfloat>>&data, GLfloat x, GLfloat y, GLfloat z);
    ~ClientLogic();
};


#endif //SHIPS_CLIENTLOGIC_H
