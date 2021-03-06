/*
    window.cpp - Window class implementation file
*/

#include "gui/window.h"

using namespace morphios::common;
using namespace morphios::gui;

Window::Window(Widget* parent, 
    common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, 
    common::uint8_t r, common::uint8_t g, common::uint8_t b) 
    : CompositeWidget(parent, x, y, w, h, r, g, b),
    dragging(false) {}

Window::~Window() {}

void Window::onMouseDown(common::int32_t x, common:: int32_t y, common::uint8_t button) {
    // True only if left click
    dragging = button == 1;

    CompositeWidget::onMouseDown(x,y,button);
}

void Window::onMouseUp(common::int32_t x, common:: int32_t y, common::uint8_t button) {
    dragging = false;
    CompositeWidget::onMouseUp(x,y,button);
}

void Window::onMouseMove(common::int32_t oldx, common:: int32_t oldy, common::int32_t newx, common:: int32_t newy) {
    if(dragging){
        this->x += newx-oldx;
        this->y += newy-oldy;
    }
    CompositeWidget::onMouseMove(oldx,oldy,newx,newy);
}


