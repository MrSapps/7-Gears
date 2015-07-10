#pragma once

#include "nanovg.h"

class Menu
{
public:
    Menu();
    void Render(NVGcontext* vg);
    void Update();
    void HandleInput();
private:
    enum eStates
    {
        eNone
    };
    eStates mState = eNone;
};
