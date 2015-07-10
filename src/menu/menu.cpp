#include "menu/menu.hpp"

namespace Menus
{
    static void RenderWindow(NVGcontext* vg, int ix, int iy, int iw, int ih)
    {
        const float x = static_cast<float>(ix);
        const float y = static_cast<float>(iy);
        const float w = static_cast<float>(iw);
        const float h = static_cast<float>(ih);

        float rounding = 6.0f;

        // black outline
        nvgResetTransform(vg);
        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGBA(123, 123, 123, 255));
        nvgRoundedRect(vg, x, y, w, h, rounding);
        nvgFill(vg);

        // white inner
        float pad1 = 2.0f;
        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGBA(222, 222, 222, 255));
        nvgRoundedRect(vg, x + pad1, y + pad1, w - pad1 - pad1, h - pad1 - pad1, rounding);
        nvgFill(vg);

        // black inner
        pad1 = 5.0f;
        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGBA(74, 74, 74, 255));
        nvgRoundedRect(vg, x + pad1, y + pad1, w - pad1 - pad1, h - pad1 - pad1, rounding);
        nvgFill(vg);

        // Gradient window fill
        float pad2 = 7.0f;
        nvgResetTransform(vg);
        nvgTranslate(vg, pad2, pad2);
        nvgBeginPath(vg);
        NVGpaint paint = nvgLinearGradient(vg, x, y, w, h, nvgRGBA(0, 0, 155, 255), nvgRGBA(0, 0, 55, 255));
        nvgFillPaint(vg, paint);
        nvgRoundedRect(vg, x, y, w - pad2 - pad2, h - pad2 - pad2, rounding);
        nvgFill(vg);
    }
}

static void DrawText(NVGcontext* vg, float xpos, float ypos, const char* msg, bool disable = false)
{

    nvgResetTransform(vg);
    nvgTranslate(vg, 0.5f, 0.5f);


    nvgFontSize(vg, 38.0f);
    nvgFontBlur(vg, 0);
    nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
    nvgText(vg, xpos, ypos, msg, nullptr);

    nvgResetTransform(vg);
    nvgFontSize(vg, 38.0f);
    nvgFontBlur(vg, 0);
    if (!disable)
    {
        nvgFillColor(vg, nvgRGBA(230, 230, 230, 255));
    }
    else
    {
        nvgFillColor(vg, nvgRGBA(94, 94, 94, 255));
    }
    nvgText(vg, xpos - 2.0f, ypos - 2.0f, msg, nullptr);

    nvgResetTransform(vg);
    nvgBeginPath(vg);
    NVGpaint paint = nvgLinearGradient(vg, 100.0f, 100.0f, 100.0f + 15, 120.0f + 15, nvgRGBA(0, 0, 0, 255), nvgRGBA(0, 255, 0, 155));
    nvgFillPaint(vg, paint);

    nvgFillPaint(vg, paint);
    nvgCircle(vg, 100.0f, 120.0f, 10.0f);
    nvgFill(vg);

}

Menu::Menu()
{

}

void Menu::Render(NVGcontext* vg)
{

    nvgBeginFrame(vg, 800, 600, 1.0f);

    nvgResetTransform(vg);

    Menus::RenderWindow(vg, 30, 430.0f, 470.0f, 60);
    DrawText(vg, 50.0f, 470.0f, "Could be the end of the world...");

    Menus::RenderWindow(vg, 100, 250, 800.0f - 100 - 100, 90);
    DrawText(vg, 120.0f, 250 + 40.0f, "Save 1    Save 2    Save 3    Save 4    Save 5");
    DrawText(vg, 120.0f, 250 + 40.0f + 30.0f, "Save 6    Save 7    Save 8    Save 9    Save 10", true);


    Menus::RenderWindow(vg, 0, 0, 800.0f-200.0f, 60);
    DrawText(vg, 20.0f, 40.0f, "Checking save data file.");

    Menus::RenderWindow(vg, 800 - 200, 0, 200, 60);
    DrawText(vg, (800 - 200) + 20.0f, 40.0f, "Load");

    // Temp cursor
    nvgResetTransform(vg);
    nvgBeginPath(vg);
    nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
    nvgRect(vg, mCursorXPos, mCursorYPos, 25, 25);
    nvgFill(vg);


    nvgEndFrame(vg);

}

void Menu::Update()
{

}

void Menu::HandleInput(const bool(&buttons)[SDL_CONTROLLER_BUTTON_MAX], const bool(&oldbuttons)[SDL_CONTROLLER_BUTTON_MAX])
{
    if (!oldbuttons[SDL_CONTROLLER_BUTTON_DPAD_LEFT] && buttons[SDL_CONTROLLER_BUTTON_DPAD_LEFT])
    {
        mCursorXPos-=40.0f;
    }

    if (!oldbuttons[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] && buttons[SDL_CONTROLLER_BUTTON_DPAD_RIGHT])
    {
        mCursorXPos += 40.0f;
    }

    if (!oldbuttons[SDL_CONTROLLER_BUTTON_DPAD_UP] && buttons[SDL_CONTROLLER_BUTTON_DPAD_UP])
    {
        mCursorYPos -= 40.0f;
    }
    
    if (!oldbuttons[SDL_CONTROLLER_BUTTON_DPAD_DOWN] && buttons[SDL_CONTROLLER_BUTTON_DPAD_DOWN])
    {
        mCursorYPos += 40.0f;
    }

}
