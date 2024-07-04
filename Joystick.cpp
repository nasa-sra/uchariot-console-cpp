#include "SDL2/SDL.h"
#include <iostream>

void joystick()
{
    // int joysticks = SDL_Init(SDL_INIT_GAMECONTROLLER);
    // std::cout << "joysticks: " << joysticks << std::endl;
    // if (joysticks < 0)
    // {
    //     std::cerr << "Failed to initialize SDL22222: " << SDL_GetError() << std::endl;
    //     return -1;
    // }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    }

    if (SDL_NumJoysticks() < 1)
    {
        std::cerr << "No joysticks connected!" << std::endl;
    }
    else
    {
        SDL_Joystick *joystick = SDL_JoystickOpen(0);
        if (joystick == nullptr)
        {
            std::cerr << "Failed to open joystick: " << SDL_GetError() << std::endl;
        }
        else
        {
            std::cout << "Joystick Name: " << SDL_JoystickName(joystick) << std::endl;
            std::cout << "Number of Axes: " << SDL_JoystickNumAxes(joystick) << std::endl;
            std::cout << "Number of Buttons: " << SDL_JoystickNumButtons(joystick) << std::endl;

            bool quit = false;
            SDL_Event e;
            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    else if (e.type == SDL_JOYAXISMOTION)
                    {
                        std::cout << "Axis " << e.jaxis.axis << " value: " << e.jaxis.value << std::endl;
                    }
                    else if (e.type == SDL_JOYBUTTONDOWN)
                    {
                        std::cout << "Button " << (int)e.jbutton.button << " pressed." << std::endl;
                    }
                    else if (e.type == SDL_JOYBUTTONUP)
                    {
                        std::cout << "Button " << (int)e.jbutton.button << " released." << std::endl;
                    }
                }
            }

            SDL_JoystickClose(joystick);
        }
    }

    SDL_Quit();
}