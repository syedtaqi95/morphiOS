/*
    widget.cpp - Widget class implementation file
*/

#include "gui/widget.h"

using namespace morphios::common;
using namespace morphios::gui;

// Widget class implementation

Widget::Widget(Widget *parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, 
        common::uint8_t r, common::uint8_t g ,common::uint8_t b) 
    : KeyboardEventHandler(),
    parent(parent), x(x), y(y), w(w), h(h), r(r), g(g), b(b), isFocusable(true) {}

Widget::~Widget() {}

void Widget::getFocus(Widget *widget) {
    // Pass control up the heirarchy till the parent widget, which contains the method to deal with it
    if (parent != 0)
        parent->getFocus(widget);
}

void Widget::modelToScreen(common::int32_t &x, common::int32_t &y) {
    // Get offset from parent
    if (parent != 0)
        parent->modelToScreen(x, y);
    
    // Add widget's offset
    x += this->x;
    y += this->y;
}

bool Widget::containsCoordinate(common::int32_t x, common::int32_t y) {
    return (this->x <= x && x < this->x + this->w && this->y <= y && y < this->y + this->h);
}

void Widget::draw(common::graphicsContext* gc) {
    //get absolute coordinates so we know where to draw
    int X = 0;
    int Y = 0;
    modelToScreen(X,Y);

    gc->putRect(X,Y,w,h, r,g,b);
}

void Widget::onMouseDown(common::int32_t x, common:: int32_t y, common::uint8_t button) {
    if (isFocusable)
        getFocus(this);
}

void Widget::onMouseUp(common::int32_t x, common:: int32_t y, common::uint8_t button) {}

void Widget::onMouseMove(common::int32_t oldx, common:: int32_t oldy, common::int32_t newx, common:: int32_t newy) {}

