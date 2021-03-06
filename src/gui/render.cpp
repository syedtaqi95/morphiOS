/*
    render.cpp - Render class
*/

#include "gui/render.h"

using namespace morphios::common;
using namespace morphios::gui;

Render::Render() {}
Render::~Render() {}

void Render::display(common::graphicsContext* gc) {
    for (int32_t ix = 0; ix < 320; ++ix) {
        for (int32_t iy = 0; iy < 200; ++iy) {
            gc->putPixel(ix, iy, frameBuffer[ix][iy].r, frameBuffer[ix][iy].g, frameBuffer[ix][iy].b);
        }
    }
}

void Render::putPixel(common::int32_t x, common::int32_t y, common::uint8_t r, common::uint8_t g, common::uint8_t b) {
    frameBuffer[x][y].r = r;
    frameBuffer[x][y].g = g;
    frameBuffer[x][y].b = b;
}


