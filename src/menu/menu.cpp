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

        nvgResetTransform(vg);
    }
}

static void DrawText(NVGcontext* vg, float xpos, float ypos, const char* msg, bool disable = false)
{

   // nvgResetTransform(vg);
   
    float fontSize = 36.0f;

    nvgTextAlign(vg, NVG_ALIGN_TOP);
    float bounds[4];
    nvgTextBounds(vg, xpos, ypos, msg, nullptr, bounds);


    nvgFontSize(vg, fontSize);
    nvgFontBlur(vg, 0);
    nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
    nvgText(vg, xpos, ypos, msg, nullptr);

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
    nvgText(vg, xpos - 2.0f, ypos - 2.0f, msg, nullptr);
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

    virtual void Render(NVGcontext* vg, WindowRect screen, WindowRect widget)
    {
        float xpos = Percent(screen.w, widget.x);
        float ypos = Percent(screen.h, widget.y);
        float w = Percent(screen.w, widget.w);
        float h = Percent(screen.h, widget.h);

        nvgBeginPath(vg);
        nvgStrokeColor(vg, nvgRGBA(mR, mG, mB, 255));
        nvgRect(vg, xpos + screen.x, ypos + screen.y, w, h);
        nvgStroke(vg);
        
    }
protected:
    unsigned char mR = 250;
    unsigned char mG = 0;
    unsigned char mB = 255;
};

class Label : public Widget
{
public:
    Label()
    {
        mR = 255;
        mG = 255;
        mB = 0;
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
            DrawText(vg, xpos+screen.x, ypos + screen.y, mText.c_str());
        }
        Widget::Render(vg, screen, widget);
    }

    void SetText(const std::string& text)
    {
        mText = text;
    }

private:
    std::string mText;
};


class Container : public Widget
{
public:
    Container()
    {

    }

    virtual void Render(NVGcontext* vg, WindowRect screen, WindowRect widget) override
    {
        if (mWidget)
        {
            mWidget->Render(vg, screen, widget);
            Widget::Render(vg, screen, widget);
        }
    }

    void SetWidget(std::unique_ptr<Widget> w)
    {
        mWidget = std::move(w);
    }

private:
    std::unique_ptr<Widget> mWidget;
};

static float ToPercent(float current, float max)
{
    return (current / max) * 100.0f;
}

class Window : public Container
{
public:
    Window()
    {

    }

    virtual void Render(NVGcontext* vg, WindowRect screen, WindowRect widget) override
    {
        float xpos = Percent(screen.w, widget.x);
        float ypos = Percent(screen.h, widget.y);
        float width = Percent(screen.w, widget.w);
        float height = Percent(screen.h, widget.h);
        mR = 0;
        mG = 255;
        mB = 0;
        Widget::Render(vg, screen, widget);
        Menus::RenderWindow(vg, xpos + screen.x, ypos + screen.y, width, height);

        // TODO: Caculate as a pixel amount via percentage of the overall window width
        // Add N pixels for padding to child, but convert back to %
        float hackWindowBorderAmount = 6.0f;
        widget.x = ToPercent(xpos + hackWindowBorderAmount, screen.w);
        widget.y = ToPercent(ypos + hackWindowBorderAmount, screen.h);
        widget.w = ToPercent(width - (hackWindowBorderAmount * 2), screen.w);
        widget.h = ToPercent(height - (hackWindowBorderAmount * 2), screen.h);


        mR = 255;
        mG = 0;
        mB = 0;
        Container::Render(vg, screen, widget);
    }

};

class Cell : public Container
{
public:
    Cell()
    {
        mR = 0;
        mG = 0;
        mB = 255;
    }

    void SetWidthHeightPercent(float wpercent, float hpercent)
    {
        mWidthPercent = wpercent;
        mHeightPercent = hpercent;
    }

    void Render(NVGcontext* vg, WindowRect screen, WindowRect widget) override
    {
        Container::Render(vg, screen, widget);
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
    float mWidthPercent = 1.0f;
    float mHeightPercent = 1.0f;
};

class TableLayout : public Container
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

    void Render(NVGcontext* vg, WindowRect screen, WindowRect widget) override
    {

        nvgResetTransform(vg);
        
        // Calc the screen rect for the whole table
        WindowRect tableRect;
        tableRect.x = Percent(screen.w, widget.x) + screen.x;
        tableRect.y = Percent(screen.h, widget.y) + screen.y;
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
        Container::Render(vg, screen, widget);
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

    WindowRect screen = { 50.0f, 50.0f, screenW-100.0f, screenH-100.0f };
   // WindowRect screen = { 0.0f, 0.0f, screenW, screenH };

    nvgBeginFrame(vg, screenW, screenH, 1.0f);


    nvgResetTransform(vg);
    
    // Screen rect
    nvgBeginPath(vg);
    nvgFillColor(vg, nvgRGBA(128, 128, 0, 255));
    nvgRect(vg, screen.x, screen.y, screen.w, screen.h);
    nvgFill(vg);

    nvgResetTransform(vg);

    TableLayout layout2(2, 1);
    layout2.GetCell(0, 0).SetWidthHeightPercent(75, 100);

    auto txt2 = std::make_unique<Window>();
    txt2->SetWidget(std::make_unique<Label>("Checking save data file."));
    layout2.GetCell(0, 0).SetWidget(std::move(txt2));
    layout2.GetCell(1, 0).SetWidthHeightPercent(25, 100);
    auto txt3 = std::make_unique<Window>();
    txt3->SetWidget(std::make_unique<Label>("Load"));
    layout2.GetCell(1, 0).SetWidget(std::move(txt3));
    layout2.Render(vg, screen, WindowRect{ 0.0f, 0.0f, 100.0f, 10.8f });
    

    // Nested table test

    {
        auto layout = std::make_unique<TableLayout>(2, 2);
        int saveNum = 0;
        for (int x = 0; x < 2; x++)
        {
            for (int y = 0; y < 2; y++)
            {
                if (x != 1 && y != 1)
                {
                    auto layout2 = std::make_unique<TableLayout>(2, 2);
                    for (int x2 = 0; x2 < 2; x2++)
                    {
                        for (int y2 = 0; y2 < 2; y2++)
                        {
                            layout2->GetCell(x2, y2).SetWidget(std::make_unique<Label>("T:" + std::to_string(++saveNum) + " " + std::to_string(x2 + 1) + "," + std::to_string(y2 + 1)));
                        }
                    }
                    layout->GetCell(x, y).SetWidget(std::move(layout2));
                }
                else
                {
                    layout->GetCell(x, y).SetWidget(std::make_unique<Label>("Other"));
                }
            }
        }

        Window win;
        win.SetWidget(std::move(layout));
        win.Render(vg, screen, WindowRect{ 15.0f, 15.0f, 100.0f - 30.0f, 100.0f - 30.0f });
    }



    TableLayout l(1, 1);
    l.GetCell(0, 0).SetWidthHeightPercent(100, 100);
    auto txt1 = std::make_unique<Window>();
    txt1->SetWidget(std::make_unique<Label>("Could be the end of the world..."));
    l.GetCell(0, 0).SetWidget(std::move(txt1));
    l.Render(vg, screen, WindowRect{ 1, 78, 65, 10 });

    auto layout = std::make_unique<TableLayout>(5, 2);
    int saveNum = 0;
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 2; y++)
        {
            layout->GetCell(x, y).SetWidget(std::make_unique<Label>("Save " + std::to_string(++saveNum)));
        }
    }

    Window saves;
    saves.SetWidget(std::move(layout));
    saves.Render(vg, screen, WindowRect{ 24.0f, 62.0f, 100.0f - (14.0f * 2), 14.0f });
    

    
    Window test;
    test.SetWidget(std::make_unique<Label>("Testing direct window"));
    test.Render(vg, screen, WindowRect{ 2.0f, 90.0f, 50.0f, 10.0f });


    

    Window nestedWin;
    auto subWin = std::make_unique<Window>();
    subWin->SetWidget(std::make_unique<Window>());
    nestedWin.SetWidget(std::move(subWin));
    nestedWin.Render(vg, screen, WindowRect{ 90.0f, 90.0f, 10.0f, 10.0f });

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
