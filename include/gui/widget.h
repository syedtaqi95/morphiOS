/*
    widget.h - MorphiOS GUI widget header file
*/

#ifndef WIDGET_H
#define WIDGET_H

#include "common/types.h"

namespace morphios {
namespace gui {

// Generic widget class
class Widget {
protected:
    Widget *parent; // Parent widget

    // Positional members
    common::int32_t x, y, w, h;

    // Colour related members
    common::uint8_t r, g, b;

    bool isFocusable;

public:
    Widget(Widget *parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, 
        common::uint8_t r, common::uint8_t g ,common::uint8_t b);
    ~Widget();

    virtual void getFocus(Widget *widget);
    virtual void modelToScreen(common::int32_t &x, common::int32_t &y);
    virtual bool containsCoordinate(common::int32_t x, common::int32_t y);

    virtual void Draw();
    virtual void OnMouseDown(common::int32_t x, common:: int32_t y, common::uint8_t button);
    virtual void OnMouseUp(common::int32_t x, common:: int32_t y, common::uint8_t button);
    virtual void OnMouseMove(common::int32_t oldx, common:: int32_t oldy, common::int32_t newx, common:: int32_t newy);
};
    
} // namespace gui    
} // namespace morphios

#endif // WIDGET_H
