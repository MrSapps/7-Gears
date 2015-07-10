#pragma once

#include <memory>
#include <map>
#include <stdlib.h>
#include <GL/glew.h>

#include "nanovg.h"

#include <SDL.h>


class Kernel;
class Menu;

class Engine
{
public:
    Engine();
    ~Engine();
    int Run();
private:
    void Update();
    void Render();
    int Init();
    int InitSDL();
    void AddExistingControllers();
    void AddController(int id);
    void RemoveController(int id);
    std::map<int, SDL_GameController*> mIdtoControllerMap;
    void OnButton(SDL_GameControllerButton button, bool down);

    void DeInit();

    void HandleInput();

    std::unique_ptr<Kernel> mKernel;
    std::unique_ptr<Menu> mMenu;
    bool mQuit = false;
    NVGcontext* vg = nullptr;
    struct NVGLUframebuffer *fb = nullptr;
    SDL_Window *mSDLWindow = nullptr;

    enum eStates
    {
        eInit,
        eMenu,
    };
    eStates mState = eInit;
};
