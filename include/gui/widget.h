/*
    widget.h - MorphiOS GUI widget header file
*/

#ifndef WIDGET_H
#define WIDGET_H

#include "common/types.h"
#include "drivers/keyboard.h"
#include "common/graphicscontext.h"

namespace morphios {
namespace gui {

// Generic widget class
class Widget : public drivers::KeyboardEventHandler {
protected:
    Widget *parent; // Parent widget    
    common::int32_t x, y, w, h; // Positional members    
    common::uint8_t r, g, b; // Colour related members
    bool isFocusable;

public:
    // Contructor
    Widget(Widget *parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, 
        common::uint8_t r, common::uint8_t g ,common::uint8_t b);
    // Destructor
    ~Widget();

    // Virtual methods
    virtual void getFocus(Widget *widget);
    virtual void modelToScreen(common::int32_t &x, common::int32_t &y);
    virtual bool containsCoordinate(common::int32_t x, common::int32_t y);
    virtual void draw(common::graphicsContext* gc);
    virtual void onMouseDown(common::int32_t x, common:: int32_t y, common::uint8_t button);
    virtual void onMouseUp(common::int32_t x, common:: int32_t y, common::uint8_t button);
    virtual void onMouseMove(common::int32_t oldx, common:: int32_t oldy, common::int32_t newx, common:: int32_t newy);
};

// Stores an array of widgets in the order they need to be drawn
class CompositeWidget : public Widget {
private:
    Widget* children[100];
    common::int32_t numChildren;
    Widget* focussedChild;

public:
    // Constructor
    CompositeWidget(Widget* parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, 
        common::uint8_t r, common::uint8_t g, common::uint8_t b);
    // Destructor
    ~CompositeWidget();

    // Virtual methods called by widgets
    virtual void getFocus(Widget *widget);
    virtual bool AddChild(Widget* child);
    virtual void draw(common::graphicsContext* gc);
    virtual void onMouseDown(common::int32_t x, common:: int32_t y, common::uint8_t button);
    virtual void onMouseUp(common::int32_t x, common:: int32_t y, common::uint8_t button);
    virtual void onMouseMove(common::int32_t oldx, common:: int32_t oldy, common::int32_t newx, common:: int32_t newy);
    virtual void OnKeyDown(char);
    virtual void OnKeyUp(char);
};
    
} // namespace gui    
} // namespace morphios

#endif // WIDGET_H
