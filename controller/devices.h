//
// Created by Michael Berner on 28/12/2025.
//

#include <fstream>
#include <iostream>
#include <SDL.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <cmath>
#include <thread>
#include <chrono>



#ifndef DETECT_CONTROLLER_DEVICES_H
#define DETECT_CONTROLLER_DEVICES_H

inline void sendPipeButton(std::string buttun, std::string pressOrRelease, int pipeID) {
    std::string message = buttun + "\n" + pressOrRelease + "\n\n";
    write(pipeID, message.c_str(), message.size());
}

inline void sendPipeAnalog(std::string stick, std::string axis, float value, int pipeID) {
    std::string message = stick + "\n" + axis + "\n" + std::to_string(value) + "\n";
    write(pipeID, message.c_str(), message.size());
}


class Controller {

    private:
    SDL_Event *event;
    SDL_GameController* controller = nullptr;
    int pipeID; //pipe to PS4/PS5

    public:
    typedef enum event {
        AXISMOTION = SDL_CONTROLLERAXISMOTION,
        BUTTONDOWN,
        BUTTONUP,
        CONTROLLERADD,
        CONTROLLERREMOVE
    };

    Controller(SDL_Event *e, int *pipe);

    void buttonDown();

    void buttonUp();

    void axisMotion();

    void controllerAdd();

    void controllerRemove();
};


#endif //DETECT_CONTROLLER_DEVICES_H