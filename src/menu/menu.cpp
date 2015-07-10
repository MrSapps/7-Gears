#include "menu/menu.hpp"
#include <deque>
#include <memory>
#include <iostream>

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

class Widget
{
public:
    Widget()
    {

    }

private:

};

class Cell
{
public:
    Cell()
    {

    }

    void SetWidthHeightPercent(float wpercent, float hpercent)
    {
        mWidthPercent = wpercent;
        mHeightPercent = hpercent;
    }

    void Render(NVGcontext* vg, float xPercentPos, float yPercentPos, float containerWidth, float containerHeight, float parentX, float parentY)
    {
        float xpos = ((containerWidth / 100.0f) * xPercentPos) + parentX;
        float ypos = ((containerHeight / 100.0f) * yPercentPos) + parentY;
        float w = (containerWidth / 100.0f) * mWidthPercent;
        float h = (containerHeight / 100.0f) * mHeightPercent;

        Menus::RenderWindow(vg, xpos, ypos, w, h);
        /*
        nvgResetTransform(vg);
        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGBA(255, 0, 255, 255));
        nvgRect(vg, xpos, ypos, w, h);
        nvgFill(vg);
        */
    }

    float WidthPercent() const
    {
        return mWidthPercent;
    }

    float HeightPercent() const
    {
        return mHeightPercent;
    }

private:
    std::unique_ptr<Widget> mWidget;
    float mWidthPercent = 1.0f;
    float mHeightPercent = 1.0f;
};

class TableLayout
{
public:
    TableLayout(float x, float y, float w, float h, int cols, int rows)
        : mXPos(x), mYPos(y), mWidth(w), mHeight(h)
    {
        // Default to 1x1 table
        mCells.resize(rows);
        for (int i = 0; i < rows; i++)
        {
            mCells[i].resize(cols);
        }
    }

    Cell& GetCell(int x, int y)
    {
        return mCells[y][x];
    }

    void Render(NVGcontext* vg)
    {
        nvgResetTransform(vg);
        nvgTranslate(vg, mXPos, mYPos);

        float yPercent = 0.0f;
        for (size_t y = 0;y < mCells.size(); y++)
        {
            float xPercent = 0.0f;
            for (size_t x = 0; x < mCells[y].size(); x++)
            {
                mCells[y][x].Render(vg, xPercent, yPercent, mWidth, mHeight, mXPos, mYPos);
                xPercent += mCells[y][x].WidthPercent();
                
            }
            yPercent += mCells[y][0].HeightPercent();
        }
    }

private:
    std::deque<std::deque<Cell>> mCells;
    float mXPos;
    float mYPos;
    float mWidth;
    float mHeight;
};

void Menu::Render(NVGcontext* vg)
{

    nvgBeginFrame(vg, 800, 600, 1.0f);

    nvgResetTransform(vg);

    Menus::RenderWindow(vg, 30, 430.0f, 470.0f, 60);
    DrawText(vg, 50.0f, 470.0f, "Could be the end of the world...");

    Menus::RenderWindow(vg, 0, 0, 800.0f-200.0f, 60);
    DrawText(vg, 20.0f, 40.0f, "Checking save data file.");

    Menus::RenderWindow(vg, 800 - 200, 0, 200, 60);
    DrawText(vg, (800 - 200) + 20.0f, 40.0f, "Load");

    Menus::RenderWindow(vg, 100, 250, 800.0f - 100 - 100, 90);
    DrawText(vg, 120.0f, 250 + 40.0f, "Save 1    Save 2    Save 3    Save 4    Save 5");
    DrawText(vg, 120.0f, 250 + 40.0f + 30.0f, "Save 6    Save 7    Save 8    Save 9    Save 10", true);
    
    TableLayout layout(150.0f, 150.0f, 600.0f, 90.0f, 5, 2);
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 2; y++)
        {
            layout.GetCell(x, y).SetWidthHeightPercent(100 / 5, 100 / 2);
        }
    }

    layout.GetCell(0, 0).SetWidthHeightPercent(5, 100 / 2);
    layout.GetCell(1, 0).SetWidthHeightPercent(20+(20-5), 100 / 2);

    layout.Render(vg);

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
        mCursorXPos -= 40.0f;
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
