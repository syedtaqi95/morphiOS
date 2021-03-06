/*
    desktop.cpp - GUI desktop class
*/

#include "gui/desktop.h"

using namespace morphios::common;
using namespace morphios::gui;

Desktop::Desktop(common::int32_t w, common::int32_t h, 
        common::uint8_t r, common::uint8_t g, common::uint8_t b)
    : CompositeWidget(0, 0, 0, w, h, r, g, b), MouseEventHandler(){
    
    // Initialise mouse in the middle of the screen
    mouseX = w / 2;
    mouseY = h / 2;
}

Desktop::~Desktop(){}

void Desktop::draw(common::graphicsContext *gc){
    // draw desktop
    CompositeWidget::draw(gc);

    // draw mouse cursor as white cross
    for (int32_t i = 0; i < 4; ++i) {
        gc->putPixel(mouseX - i, mouseY, 0xFF, 0xFF, 0xFF);
        gc->putPixel(mouseX + i, mouseY, 0xFF, 0xFF, 0xFF);
        gc->putPixel(mouseX, mouseY - i, 0xFF, 0xFF, 0xFF);
        gc->putPixel(mouseX, mouseY + i, 0xFF, 0xFF, 0xFF);
    }
}

void Desktop::onMouseDown(common::uint8_t button){
    CompositeWidget::onMouseDown(mouseX, mouseY, button);
}

void Desktop::onMouseUp(common::uint8_t button){
    CompositeWidget::onMouseUp(mouseX, mouseY, button);
}

void Desktop::onMouseMove(common::int32_t x, common:: int32_t y){
    // Slow down mouse movement for desired sensativity
    x /= 4;
    y /= 4;

    // Limit cursor within screen
    int32_t newMouseX = mouseX + x;
    if(newMouseX < 0) newMouseX = 0;
    if(newMouseX >= w) newMouseX = w - 1;
    int32_t newMouseY = mouseY + y;
    if(newMouseY < 0) newMouseY = 0;
    if(newMouseY >= h) newMouseY = h - 1;

    // Pass control to CompositeWidget
    CompositeWidget::onMouseMove(mouseX, mouseY, newMouseX, newMouseY);

    // Store new values
    mouseX = newMouseX;
    mouseY = newMouseY;
}




