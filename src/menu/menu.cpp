#include "menu/menu.hpp"
#include <deque>
#include <memory>
#include <iostream>
#include <string>

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
   // nvgTranslate(vg, 0.5f, 0.5f);

    float fontSize = 36.0f;

    nvgFontSize(vg, fontSize);
    nvgFontBlur(vg, 0);
    nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
    nvgText(vg, xpos, ypos + fontSize, msg, nullptr);

    nvgResetTransform(vg);
    nvgFontSize(vg, fontSize);
    nvgFontBlur(vg, 0);
    if (!disable)
    {
        nvgFillColor(vg, nvgRGBA(230, 230, 230, 255));
    }
    else
    {
        nvgFillColor(vg, nvgRGBA(94, 94, 94, 255));
    }
    nvgText(vg, xpos - 2.0f, ypos - 2.0f + fontSize, msg, nullptr);

    /*
    nvgResetTransform(vg);
    nvgBeginPath(vg);
    NVGpaint paint = nvgLinearGradient(vg, 100.0f, 100.0f, 100.0f + 15, 120.0f + 15, nvgRGBA(0, 0, 0, 255), nvgRGBA(0, 255, 0, 155));
    nvgFillPaint(vg, paint);

    nvgFillPaint(vg, paint);
    nvgCircle(vg, 100.0f, 120.0f, 10.0f);
    nvgFill(vg);
    */
}

Menu::Menu()
{

}

static float Percent(float max, float percent)
{
    return (max / 100.0f) * percent;
}

struct WindowRect
{
    float x, y, w, h;
};

class Widget
{
public:
    Widget()
    {

    }

    virtual ~Widget()
    {

    }

    virtual void Render(NVGcontext* vg, WindowRect screen, WindowRect widget) = 0;
private:

};

class Label : public Widget
{
public:
    Label()
    {

    }

    Label(const std::string& text)
        : mText(text)
    {

    }

    virtual void Render(NVGcontext* vg, WindowRect screen, WindowRect widget) override
    {
        if (!mText.empty())
        {
            float xpos = Percent(screen.w, widget.x);
            float ypos = Percent(screen.h, widget.y);
            DrawText(vg, xpos+ screen.x + 15, ypos + screen.y, mText.c_str());
        }
    }

    void SetText(const std::string& text)
    {
        mText = text;
    }

private:
    std::string mText;
};

class Window : public Widget
{
public:
    Window()
    {

    }

    Window(const std::string& text)
        : mText(text)
    {

    }

    virtual void Render(NVGcontext* vg, WindowRect screen, WindowRect widget) override
    {
         float xpos = Percent(screen.w, widget.x);
         float ypos = Percent(screen.h, widget.y);
         float width = Percent(screen.w, widget.w);
         float height = Percent(screen.h, widget.h);

         Menus::RenderWindow(vg, xpos + screen.x, ypos + screen.y, width, height);
         if (!mText.empty())
         {
             DrawText(vg, xpos + screen.x + 15, ypos + screen.y, mText.c_str());
         }
    }
private:
    std::string mText;
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

    void Render(NVGcontext* vg, WindowRect screen, WindowRect widget)
    {
        if (mWidget)
        {
            mWidget->Render(vg,screen, widget);
        }
    }

    float WidthPercent() const
    {
        return mWidthPercent;
    }

    float HeightPercent() const
    {
        return mHeightPercent;
    }

    void SetWidget(std::unique_ptr<Widget> w)
    {
        mWidget = std::move(w);
    }

private:
    std::unique_ptr<Widget> mWidget;
    float mWidthPercent = 1.0f;
    float mHeightPercent = 1.0f;
};

class TableLayout : public Widget
{
public:
    TableLayout(int cols, int rows)
    {
        // Set up the table/grid
        mCells.resize(rows);
        for (int i = 0; i < rows; i++)
        {
            mCells[i].resize(cols);
        }

        // Make each cell take equal size
        for (int x = 0; x < cols; x++)
        {
            for (int y = 0; y < rows; y++)
            {
                GetCell(x, y).SetWidthHeightPercent(100.0f / cols, 100.0f / rows);
            }
        }
    }

    Cell& GetCell(int x, int y)
    {
        return mCells[y][x];
    }

    void Render(NVGcontext* vg, WindowRect screen, WindowRect widget)
    {
        nvgResetTransform(vg);
        
        // Calc the screen rect for the whole table
        WindowRect tableRect;
        tableRect.x = Percent(screen.w, widget.x);
        tableRect.y = Percent(screen.h, widget.y);
        tableRect.w = Percent(screen.w, widget.w);
        tableRect.h = Percent(screen.h, widget.h);


        float yPercent = 0.0f;
        for (size_t y = 0; y < mCells.size(); y++)
        {
            float xPercent = 0.0f;
            for (size_t x = 0; x < mCells[y].size(); x++)
            {
                mCells[y][x].Render(vg, tableRect, WindowRect{ xPercent, yPercent, mCells[y][x].WidthPercent(), mCells[y][x].HeightPercent() });
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
    float screenW = 800.0f;
    float screenH = 600.0f;

    WindowRect screen = { 0.0f, 0.0f, screenW, screenH };

    nvgBeginFrame(vg, screenW, screenH, 1.0f);

    nvgResetTransform(vg);


    TableLayout l(1, 1);
    l.GetCell(0, 0).SetWidthHeightPercent(100, 100);
    l.GetCell(0, 0).SetWidget(std::make_unique<Window>("Could be the end of the world..."));
    l.Render(vg, screen, WindowRect{ 2, 70, 55, 8 });

    TableLayout layout2(2, 1);
    layout2.GetCell(0, 0).SetWidthHeightPercent(75, 100);
    layout2.GetCell(0, 0).SetWidget(std::make_unique<Window>("Checking save data file."));
    layout2.GetCell(1, 0).SetWidthHeightPercent(25, 100);
    layout2.GetCell(1, 0).SetWidget(std::make_unique<Window>("Load"));
    layout2.Render(vg, screen, WindowRect{ 0.0f, 0.0f, 100.0f, 10.8f });

    TableLayout layout(5, 2);
//    layout.SetWidget(std::make_unique<Window>());
    int saveNum = 0;
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 2; y++)
        {
            layout.GetCell(x, y).SetWidget(std::make_unique<Label>("Save " + std::to_string(++saveNum)));
        }
    }
    layout.Render(vg, screen, WindowRect{ 14.0f, 42.0f, 100.0f - (14.0f * 2), 14.0f });
    
    Window test("Testing");
    test.Render(vg, screen, WindowRect{ 40.0f, 80.0f, 30.0f, 9.0f });

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
