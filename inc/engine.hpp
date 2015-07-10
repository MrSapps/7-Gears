#pragma once

#include <memory>
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
    void DeInit();

    void HandleInput();

    std::unique_ptr<Kernel> mKernel;
    std::unique_ptr<Menu> mMenu;
    bool mQuit = false;
    NVGcontext* vg = nullptr;
    struct NVGLUframebuffer *fb = nullptr;
    SDL_Window *window = nullptr;
};
