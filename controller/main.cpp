#include "devices.h"


int main() {

    bool quit = false;

    SDL_Event e;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_SENSOR) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return -1;
    }


    SDL_Window* window = SDL_CreateWindow(
     "CLI Listener",
     0, 0, 1, 1,
     SDL_WINDOW_MINIMIZED
 );
    SDL_RaiseWindow(window);

    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_SetWindowInputFocus(window);



    // OPEN PIPE
    const char* pipe_path = "/tmp/my_pipe"; //todo rename pipe

    std::cout << "1" << std::endl;

    int fd = open(pipe_path, O_WRONLY);  // open ONCE
    if (fd == -1) return 1;

    Controller controller(&e, &fd);

    std::string message;

    std::cout << "Listening for keyboard events (press 'q' to quit)..." << std::endl;

    while( !quit ) {
        while (SDL_PollEvent( &e ) != 0) {

            switch (e.type) {
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                            message = "CIRCLE";
                            break;
                    case SDLK_a:
                            message = "LEFT";
                            std::cout << "message written: "<< "LEFT" << std::endl;
                            break;
                    case SDLK_d:
                            message = "RIGHT";
                            std::cout << "message written: "<< "RIGHT" << std::endl;
                            break;
                    case SDLK_w:
                            message = "UP";
                            std::cout << "message written: "<< "UP" << std::endl;
                            break;
                    case SDLK_s:
                            message = "DOWN";
                            std::cout << "message written: "<< "DOWN" << std::endl;
                            break;
                    case SDLK_RETURN:
                            message = "CROSS";
                            break;
                    case SDLK_q:
                            quit = true;
                            printf("Quitting\n");
                            break;

                    }
                    sendPipeButton(message,"press", fd);
                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                    sendPipeButton(message,"release", fd);

                    // --- Handle Button Presses ---
                case controller.BUTTONDOWN:
                    controller.buttonDown();
                    break;
                case controller.BUTTONUP:
                    controller.buttonUp();
                    break;
                    // --- Handle Axis Motion (Joysticks & Triggers) ---
                case controller.AXISMOTION:
                    controller.axisMotion();
                    break;

                        // --- Hot-plugging Support ---
                    case controller.CONTROLLERADD:
                        controller.controllerAdd();
                        break;

                    case controller.CONTROLLERREMOVE:
                        controller.controllerRemove();
                        break;
                    }
            }
        }
    SDL_Delay(10);
    std::cout << "Finished" << std::endl;
    SDL_Quit();
}