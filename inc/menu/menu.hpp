#pragma once

#include "nanovg.h"
#include <memory>
#include <SDL.h>

class Menu
{
public:
    Menu();
    ~Menu();
    void Render(NVGcontext* vg);
    void Update();
    void HandleInput(const bool(&buttons)[SDL_CONTROLLER_BUTTON_MAX], const bool(&oldbuttons)[SDL_CONTROLLER_BUTTON_MAX]);
private:
    enum eStates
    {
        eNone
    };
    eStates mState = eNone;

    void TestUi(const struct WindowRect& screen, NVGcontext* vg);

    std::unique_ptr<class SelectionGrid> mSaves;

};
