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

int sdl_init(SDL_Window **w)
{
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) == 0)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        //SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG ); // May be a performance booster in *nix?
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        *w = SDL_CreateWindow("7-Gears", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL);
        if (*w)
        {
            SDL_GL_CreateContext(*w);

            // Activate glew
            glewExperimental = GL_TRUE;
            GLenum err = glewInit();
            if (err == GLEW_OK)
            {
                glEnable(GL_STENCIL_TEST);

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
    if (*w) SDL_DestroyWindow(*w);
    *w = NULL;
    return 1;
}

void sdl_cleanup()
{
    SDL_Quit();
}

Engine::Engine()
{
    mKernel = std::make_unique<Kernel>();
    mMenu = std::make_unique<Menu>();
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

    while (!mQuit)
    {
        Update();
        Render();
    }
    return 0;
}

void Engine::Update()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_WINDOWEVENT:
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_MAXIMIZED:
            case SDL_WINDOWEVENT_RESTORED:
                OnResize(window);
                break;
            }
            break;

        case SDL_KEYDOWN:
            if (e.key.keysym.sym == 13)
            {
                const Uint32 windowFlags = SDL_GetWindowFlags(window);
                bool isFullScreen = ((windowFlags & SDL_WINDOW_FULLSCREEN_DESKTOP) || (windowFlags & SDL_WINDOW_FULLSCREEN));
                SDL_SetWindowFullscreen(window, isFullScreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
                OnResize(window);
            }
            break;

        case SDL_KEYUP:
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


}

void Engine::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    nvgBeginFrame(vg, 800, 600, 1.0f);

    nvgResetTransform(vg);


    Menus::RenderWindow(vg, 30, 430.0f, 470.0f, 60);
    DrawText(vg, 50.0f, 470.0f, "Could be the end of the world...");

    Menus::RenderWindow(vg, 100, 250, 800.0f - 100 - 100, 90);
    DrawText(vg, 120.0f, 250 + 40.0f, "Save 1    Save 2    Save 3    Save 4    Save 5");
    DrawText(vg, 120.0f, 250 + 40.0f + 30.0f, "Save 6    Save 7    Save 8    Save 9    Save 10", true);


    Menus::RenderWindow(vg, 0, 0, 800.0f, 60);
    DrawText(vg, 20.0f, 40.0f, "Checking save data file.");

    Menus::RenderWindow(vg, 800 - 200, 0, 200, 60);
    DrawText(vg, (800 - 200) + 20.0f, 40.0f, "Load");

    nvgEndFrame(vg);
    nvgluBindFramebuffer(NULL);

    SDL_GL_SwapWindow(window);

}

int Engine::Init()
{
    if (sdl_init(&window) != 0)
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

void Engine::DeInit()
{
    nvgluDeleteFramebuffer(fb);
    nvgDeleteGL3(vg);

    sdl_cleanup();
}

void Engine::HandleInput()
{

}
