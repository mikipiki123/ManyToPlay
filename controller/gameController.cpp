//
// Created by Michael Berner on 28/12/2025.
//

#include "devices.h"

#define UPPER_THRESHOLD 32000
#define LOWER_THRESHOLD 200
#define DEAD_ZONE = 8000

Controller::Controller(SDL_Event *e, int *pipe) {
    event = e;
    pipeID = *pipe;

    if (SDL_NumJoysticks() < 1) {
        std::cout << "Warning: No joysticks connected!" << std::endl;
    } else {
        // Open the first available controller
        for (int i = 0; i < SDL_NumJoysticks(); ++i) {
            if (SDL_IsGameController(i)) {
                controller = SDL_GameControllerOpen(i);
                if (controller) {
                    std::cout << "Controller connected: " << SDL_GameControllerName(controller) << std::endl;

                    // Optional: Enable Gyroscope and Accelerometer for DualSense
                    SDL_GameControllerSetSensorEnabled(controller, SDL_SENSOR_GYRO, SDL_TRUE);
                    SDL_GameControllerSetSensorEnabled(controller, SDL_SENSOR_ACCEL, SDL_TRUE);

                    // Optional: Set LED Color to Purple (R:255, G:0, B:255)
                    SDL_GameControllerSetLED(controller, 255, 0, 255);
                    break;
                } else {
                    std::cerr << "Could not open game controller " << i << ": " << SDL_GetError() << std::endl;
                }
            }
        }
    }
}

void Controller::buttonDown() {
    switch (event->cbutton.button) {
                            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                                sendPipeButton("LEFT", "press",pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                                sendPipeButton("RIGHT","press", pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                                sendPipeButton("UP","press", pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                                sendPipeButton("DOWN","press", pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_A:
                                sendPipeButton("CROSS","press", pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_B:
                                sendPipeButton("CIRCLE","press", pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_X:
                                sendPipeButton("SQUARE","press", pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_Y:
                                sendPipeButton("TRIANGLE", "press",pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_LEFTSTICK:
                                sendPipeButton("L3", "press",pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
                                sendPipeButton("R3", "press",pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_START:
                                sendPipeButton("OPTIONS", "press",pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_GUIDE:
                                sendPipeButton("PS", "press",pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                                sendPipeButton("L1", "press", pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                                sendPipeButton("R1", "press", pipeID);
                                break;
                        }
}

void Controller::buttonUp() {
    switch (event->cbutton.button) {
                            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                                sendPipeButton("LEFT","release", pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                                sendPipeButton("RIGHT","release", pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                                sendPipeButton("UP","release", pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                                sendPipeButton("DOWN","release", pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_A:
                                sendPipeButton("CROSS","release", pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_B:
                                sendPipeButton("CIRCLE", "release",pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_X:
                                sendPipeButton("SQUARE","release", pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_Y:
                                sendPipeButton("TRIANGLE","release", pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_LEFTSTICK:
                                sendPipeButton("L3", "release",pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
                                sendPipeButton("R3", "release",pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_START:
                                sendPipeButton("OPTIONS", "release",pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_GUIDE:
                                sendPipeButton("PS", "release",pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                                sendPipeButton("L1", "release", pipeID);
                                break;
                            case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                                sendPipeButton("R1", "release", pipeID);
                        }
}

void Controller::axisMotion() {
    switch (event->caxis.axis) {
                            case SDL_CONTROLLER_AXIS_LEFTX:
                                sendPipeAnalog("LEFT","x",(float)event->caxis.value/32768, pipeID);
                                break;
                            case SDL_CONTROLLER_AXIS_LEFTY:
                                sendPipeAnalog("LEFT","y",(float)event->caxis.value/32768, pipeID);
                                break;
                            case SDL_CONTROLLER_AXIS_RIGHTX:
                                sendPipeAnalog("RIGHT","x",(float)event->caxis.value/32768, pipeID);
                                break;
                            case SDL_CONTROLLER_AXIS_RIGHTY:
                                sendPipeAnalog("RIGHT","y",(float)event->caxis.value/32768, pipeID);
                                break;
                            case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                                if (std::abs(event->caxis.value) > UPPER_THRESHOLD) {
                                    sendPipeButton("R2","press",pipeID);
                                    // std::cout << "R2 VALUE = 1" << std::endl;
                                } else if (std::abs(event->caxis.value) < LOWER_THRESHOLD) {
                                    sendPipeButton("R2","release",pipeID);
                                    // std::cout << "R2 VALUE = 0" <<  std::endl;
                                }
                                break;
                            case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                                if (std::abs(event->caxis.value) > UPPER_THRESHOLD) { //todo change the seconds to mutex
                                    sendPipeButton("L2","press",pipeID);
                                    // std::cout << "L2 VALUE = 1" << std::endl;
                                } else if (std::abs(event->caxis.value) < LOWER_THRESHOLD) {
                                    sendPipeButton("L2","release",pipeID);
                                    // std::cout << "L2 VALUE = 0" <<  std::endl;
                                }
                                break;
                        }
}

void Controller::controllerAdd() {
    if (!controller) {
        controller = SDL_GameControllerOpen(event->cdevice.which);
        std::cout << "Controller attached!" << std::endl;
    }
}

void Controller::controllerRemove() {
    if (controller && event->cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller))) {
        SDL_GameControllerClose(controller);
        controller = nullptr;
        std::cout << "Controller disconnected!" << std::endl;
    }
}

