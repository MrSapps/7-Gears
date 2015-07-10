#include "engine.hpp"
#include "kernel/kernel.hpp"
#include "menu/menu.hpp"
#include <stdio.h>
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"

int loadFonts(NVGcontext* vg)
{
    int font = nvgCreateFont(vg, "sans", "data/Roboto-Regular.ttf");
    if (font == -1)
    {
        printf("Could not add font regular.\n");
        return -1;
    }

    font = nvgCreateFont(vg, "sans-bold", "data/Roboto-Bold.ttf");
    if (font == -1) 
    {
        printf("Could not add font bold.\n");
        return -1;
    }
    return 0;
}

static void OnResize(SDL_Window* window)
{
    int width = 0;
    int height = 0;
    SDL_GetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);
}


void sdl_cleanup()
{
    SDL_Quit();
}

Engine::Engine()
{
    mKernel = std::make_unique<Kernel>();
    mMenu = std::make_unique<Menu>();

    // TODO: Come up with a sane mapping
    mKeyBoardToControllerMap[SDL_SCANCODE_SPACE] = SDL_CONTROLLER_BUTTON_B;
    mKeyBoardToControllerMap[SDL_SCANCODE_UP] = SDL_CONTROLLER_BUTTON_DPAD_UP;
    mKeyBoardToControllerMap[SDL_SCANCODE_DOWN] = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
    mKeyBoardToControllerMap[SDL_SCANCODE_LEFT] = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
    mKeyBoardToControllerMap[SDL_SCANCODE_RIGHT] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
    mKeyBoardToControllerMap[SDL_SCANCODE_A] = SDL_CONTROLLER_BUTTON_A;
    mKeyBoardToControllerMap[SDL_SCANCODE_X] = SDL_CONTROLLER_BUTTON_X;
    mKeyBoardToControllerMap[SDL_SCANCODE_Y] = SDL_CONTROLLER_BUTTON_Y;
    mKeyBoardToControllerMap[SDL_SCANCODE_BACKSPACE] = SDL_CONTROLLER_BUTTON_BACK;
    mKeyBoardToControllerMap[SDL_SCANCODE_HOME] = SDL_CONTROLLER_BUTTON_GUIDE;
    /*
    mKeyBoardToControllerMap[SDL_SCANCODE_SPACE] = SDL_CONTROLLER_BUTTON_START;
    mKeyBoardToControllerMap[SDL_SCANCODE_SPACE] = SDL_CONTROLLER_BUTTON_LEFTSTICK;
    mKeyBoardToControllerMap[SDL_SCANCODE_SPACE] = SDL_CONTROLLER_BUTTON_RIGHTSTICK;
    mKeyBoardToControllerMap[SDL_SCANCODE_SPACE] = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
    mKeyBoardToControllerMap[SDL_SCANCODE_SPACE] = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
    */
}

Engine::~Engine()
{
    DeInit();
}

int Engine::Run()
{
    int ret = Init();
    if (ret != 0)
    {
        return ret;
    }

    mState = eMenu;

    while (!mQuit)
    {
        Update();
        Render();
    }
    return 0;
}

void Engine::AddExistingControllers()
{
    for (int i = 0; i < SDL_NumJoysticks(); ++i)
    {
        AddController(i);
    }
}

void Engine::AddController(int i)
{
    if (SDL_IsGameController(i))
    {
        SDL_GameController* controller = SDL_GameControllerOpen(i);
        if (controller)
        {
            SDL_Joystick *joy = SDL_GameControllerGetJoystick(controller);
            int instanceID = SDL_JoystickInstanceID(joy);
            mIdtoControllerMap[instanceID] = controller;
        }
        else
        {
            fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
        }
    }
}

void Engine::RemoveController(int id)
{
    auto it = mIdtoControllerMap.find(id);
    if (it != std::end(mIdtoControllerMap))
    {
        SDL_GameController *pad = it->second;
        SDL_GameControllerClose(pad);
    }
}

void Engine::Update()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_CONTROLLERDEVICEADDED:
            AddController(e.cdevice.which);
            break;

        case SDL_CONTROLLERDEVICEREMOVED:
            RemoveController(e.cdevice.which);
            break;

        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            OnButton(static_cast<SDL_GameControllerButton>(e.cbutton.button), e.type == SDL_CONTROLLERBUTTONDOWN);
            break;

        case SDL_CONTROLLERAXISMOTION:
            break;

        case SDL_WINDOWEVENT:
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_MAXIMIZED:
            case SDL_WINDOWEVENT_RESTORED:
                OnResize(mSDLWindow);
                break;
            }
            break;

        case SDL_KEYUP:
        case SDL_KEYDOWN:
        {
            if (e.key.keysym.scancode == SDL_SCANCODE_RETURN && e.type == SDL_KEYDOWN)
            {
                const Uint32 windowFlags = SDL_GetWindowFlags(mSDLWindow);
                bool isFullScreen = ((windowFlags & SDL_WINDOW_FULLSCREEN_DESKTOP) || (windowFlags & SDL_WINDOW_FULLSCREEN));
                SDL_SetWindowFullscreen(mSDLWindow, isFullScreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
                OnResize(mSDLWindow);
            }

            // We map keyboard keys onto the SDL controller keys, then game logic just handles game pad keys
            auto it = mKeyBoardToControllerMap.find(e.key.keysym.scancode);
            if (it != std::end(mKeyBoardToControllerMap))
            {
                OnButton(it->second, e.type == SDL_KEYDOWN);
            }
        }
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            break;
        case SDL_MOUSEMOTION:
            break;
        case SDL_MOUSEWHEEL:
            break;
        case SDL_QUIT:
            mQuit = true;
            break;
        }
    }

    switch (mState)
    {
    case eMenu:
        mMenu->Update();
    }

}

void Engine::OnButton(SDL_GameControllerButton button, bool down)
{
    switch (button)
    {
    case SDL_CONTROLLER_BUTTON_A:
        printf("SDL_CONTROLLER_BUTTON_A\n");
        break;

    case SDL_CONTROLLER_BUTTON_B:
        printf("SDL_CONTROLLER_BUTTON_B\n");
        break;

    case SDL_CONTROLLER_BUTTON_X:
        printf("SDL_CONTROLLER_BUTTON_X\n");
        break;

    case SDL_CONTROLLER_BUTTON_Y:
        printf("SDL_CONTROLLER_BUTTON_Y\n");
        break;

    case SDL_CONTROLLER_BUTTON_BACK:
        printf("SDL_CONTROLLER_BUTTON_BACK\n");
        break;

    case SDL_CONTROLLER_BUTTON_GUIDE:
        printf("SDL_CONTROLLER_BUTTON_GUIDE\n");
        break;

    case SDL_CONTROLLER_BUTTON_START:
        printf("SDL_CONTROLLER_BUTTON_START\n");
        break;

    case SDL_CONTROLLER_BUTTON_LEFTSTICK:
        printf("SDL_CONTROLLER_BUTTON_LEFTSTICK\n");
        break;

    case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
        printf("SDL_CONTROLLER_BUTTON_RIGHTSTICK\n");
        break;

    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
        printf("SDL_CONTROLLER_BUTTON_LEFTSHOULDER\n");
        break;

    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
        printf("SDL_CONTROLLER_BUTTON_RIGHTSHOULDER\n");
        break;

    case SDL_CONTROLLER_BUTTON_DPAD_UP:
        printf("SDL_CONTROLLER_BUTTON_DPAD_UP\n");
        break;

    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        printf("SDL_CONTROLLER_BUTTON_DPAD_DOWN\n");
        break;

    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        printf("SDL_CONTROLLER_BUTTON_DPAD_LEFT\n");
        break;

    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        printf("SDL_CONTROLLER_BUTTON_DPAD_RIGHT\n");
        break;
    };

    HandleInput(button, down);
}

void Engine::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    switch (mState)
    {
    case eMenu:
        mMenu->Render(vg);
        break;
    }

    nvgluBindFramebuffer(nullptr);
    SDL_GL_SwapWindow(mSDLWindow);
}

int Engine::Init()
{
    if (InitSDL() != 0)
    {
        // Report SDL's error
        printf("Couldn't init SDL: %s\n", SDL_GetError());
        return 1;
    }


    printf("Creating nanovg context\n");
    vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    if (!vg)
    {
        printf("Couldn't create nanovg gl3 context\n");
        // Couldn't create context
        sdl_cleanup();
        return 2;
    }

    printf("Creating nanovg framebuffer\n");
    fb = nvgluCreateFramebuffer(vg, 600, 600, 0);
    if (!fb)
    {
        // Couldn't create framebuffer
        printf("Couldn't create nanovg framebuffer\n");
        sdl_cleanup();
        return 3;
    }

    if (loadFonts(vg) != 0)
    {
        printf("Failed to load fonts\n");
        return 4;
    }

    printf("Nanovg initialized!\n");

    return 0;
}

int Engine::InitSDL()
{
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) == 0)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        //SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG ); // May be a performance booster in *nix?
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        mSDLWindow = SDL_CreateWindow("7-Gears", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL);
        if (mSDLWindow)
        {
            SDL_GL_CreateContext(mSDLWindow);

            // Activate glew
            glewExperimental = GL_TRUE;
            GLenum err = glewInit();
            if (err == GLEW_OK)
            {
                glEnable(GL_STENCIL_TEST);
                SDL_ShowCursor(0);

                return 0;
            }
            else
            {
                // Send GLEW error as an SDL Error
                // SDL's error should be clear, since any previous error would stop further initialization
                SDL_SetError("GLEW Reported an Error: %s", glewGetErrorString(err));
            }
        }
    }
 
    return 1;
}

void Engine::DeInit()
{
    nvgluDeleteFramebuffer(fb);
    nvgDeleteGL3(vg);

    sdl_cleanup();
}

void Engine::HandleInput(SDL_GameControllerButton button, bool down)
{
    mMenu->HandleInput(button, down);
}
