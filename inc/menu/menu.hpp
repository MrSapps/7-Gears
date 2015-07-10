#pragma once

#include "nanovg.h"
#include <SDL.h>

class Menu
{
public:
    Menu();
    void Render(NVGcontext* vg);
    void Update();
    void HandleInput(SDL_GameControllerButton button, bool down);
private:
    enum eStates
    {
        eNone
    };
    eStates mState = eNone;
};
