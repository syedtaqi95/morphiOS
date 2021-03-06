/*
    widget.cpp - Widget class implementation file
*/

#include "gui/widget.h"

using namespace morphios::common;
using namespace morphios::gui;

// Widget class

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

// CompositeWidget class

// Constructor
CompositeWidget::CompositeWidget(Widget* parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, 
    common::uint8_t r, common::uint8_t g, common::uint8_t b) 
    : Widget(parent, x, y, w, h, r, g, b),
    focusedChild(0),
    numChildren(0) {}

// Destructor
CompositeWidget::~CompositeWidget() {}

void CompositeWidget::getFocus(Widget *widget) {
    focusedChild = widget;
    if (parent != 0) {
        parent->getFocus(this);
    }
}

bool CompositeWidget::addChild(Widget* child) {
    if (numChildren >= 100)
        return false;
    children[numChildren++] = child;
    return true;
}

void CompositeWidget::draw(common::graphicsContext* gc) {
    // Draw background
    Widget::draw(gc);

    // Draw children
    for (uint8_t i = 0; i < numChildren; ++i) {
        children[i]->draw(gc);
    }

}

// Iterate through children and pass event to the child that contains the coordinate
// Pass the click to the one that has been clicked
void CompositeWidget::onMouseDown(common::int32_t x, common:: int32_t y, common::uint8_t button) {
    for(int i = 0; i < numChildren; ++i){
        if(children[i]->containsCoordinate(x - this->x, y - this->y)){
            children[i]->onMouseDown(x - this->x, y - this->y, button);

            // only the top window is clicked on
            break;
        }
    }
}

// Iterate through children and pass event to the child that contains the coordinate
// Pass the click to the one that has been clicked
void CompositeWidget::onMouseUp(common::int32_t x, common:: int32_t y, common::uint8_t button) {
    for(int i = 0; i < numChildren; ++i){
        if(children[i]->containsCoordinate(x - this->x, y - this->y)){
            children[i]->onMouseUp(x - this->x, y - this->y, button);

            // only the top window is clicked on
            break;
        }
    }
}

void CompositeWidget::onMouseMove(common::int32_t oldx, common:: int32_t oldy, common::int32_t newx, common:: int32_t newy) {
    int firstChild = -1;

    // for object that contains old coordinate
    for(int i = 0; i < numChildren; ++i){
        if(children[i]->containsCoordinate(oldx - this->x, oldy - this->y)){
            // subtracting thisx/y turns the coordinates into relative coordinates
            children[i]->onMouseMove(oldx - this->x, oldy - this->y, newx - this->x, newy - this->y);
            firstChild = i;

            // only the top window is clicked on
            break;
        }
    }

    // for object that contains new coordinate
    for(int i = 0; i < numChildren; ++i){
        if(children[i]->containsCoordinate(newx - this->x, newy - this->y)){
            if(firstChild != i)
                // subtracting thisx/y turns the coordinates into relative coordinates
                children[i]->onMouseMove(oldx - this->x, oldy - this->y, newx - this->x, newy - this->y);

            // only the top window is clicked on
            break;
        }
    }
}

void CompositeWidget::onKeyDown(char str) {
    if(focusedChild != 0)
        focusedChild->onKeyDown(str);
}

void CompositeWidget::onKeyUp(char str) {
    if(focusedChild != 0)
        focusedChild->onKeyUp(str);
}

