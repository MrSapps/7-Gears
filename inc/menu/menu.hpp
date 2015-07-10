#pragma once

#include "nanovg.h"
#include <SDL.h>

class Menu
{
public:
    Menu();

    void Render(NVGcontext* vg);
    void Update();
    void HandleInput(const bool(&buttons)[SDL_CONTROLLER_BUTTON_MAX], const bool(&oldbuttons)[SDL_CONTROLLER_BUTTON_MAX]);
private:
    enum eStates
    {
        eNone
    };
    eStates mState = eNone;

    float mCursorXPos = 150.0f;
    float mCursorYPos = 150.0f;
};
