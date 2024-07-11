#include <atomic>
#include <iostream>
#include <thread>

#include "SDL2/SDL.h"

class Joystick {
   public:
    void init();
    void readJoystickInput(std::atomic<bool>& running, std::atomic<double>& sideAxis, std::atomic<double>& forwardsAxis);
    void cleanUp();
};