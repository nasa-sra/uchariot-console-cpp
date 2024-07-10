#include "Joystick.h"

void Joystick::init() {
    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    }
}

void Joystick::readJoystickInput(std::atomic<bool>& running, std::atomic<double>& sideAxis, std::atomic<double>& forwardsAxis) {
    if (SDL_NumJoysticks() < 1) {
        std::cerr << "No joysticks connected!" << std::endl;
    } else {
        SDL_Joystick* joystick = SDL_JoystickOpen(0);
        if (joystick == nullptr) {
            std::cerr << "Failed to open joystick: " << SDL_GetError() << std::endl;
        } else {
            std::cout << "Joystick Name: " << SDL_JoystickName(joystick) << std::endl;
            std::cout << "Number of Axes: " << SDL_JoystickNumAxes(joystick)
                      << std::endl;
            std::cout << "Number of Buttons: " << SDL_JoystickNumButtons(joystick)
                      << std::endl;

            bool quit = false;
            SDL_Event e;
            while (running) {
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } else if (e.type == SDL_JOYAXISMOTION) {
                        if (e.jaxis.axis == 0) {
                            sideAxis.store((double)(e.jaxis.value) / 32768.);
                        } else if (e.jaxis.axis == 1) {
                            forwardsAxis.store(-(double)(e.jaxis.value) / 32768.);
                        }

                        // if (e.jaxis.axis == 1) {
                        //     std::cout << (e.jaxis.axis == 0 ? "Sideways" : "Forwards") << " Axis Value: " << e.jaxis.value
                        //               << std::endl;
                        // }

                    } else if (e.type == SDL_JOYBUTTONDOWN) {
                        std::cout << "Button " << (int)e.jbutton.button << " pressed."
                                  << std::endl;
                    } else if (e.type == SDL_JOYBUTTONUP) {
                        std::cout << "Button " << (int)e.jbutton.button << " released."
                                  << std::endl;
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            std::cout << "Joystick quit!\n";
            SDL_JoystickClose(joystick);
        }
    }
}

void Joystick::cleanUp() {
    SDL_Quit();
}