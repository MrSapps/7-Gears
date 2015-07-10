#pragma once

#include "nanovg.h"


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

class Menu
{
public:

};
