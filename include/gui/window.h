/*
    window.h - MorphiOS GUI window header file
*/

#ifndef WINDOW_H
#define WINDOW_H

#include <drivers/mouse.h>
#include <gui/widget.h>

namespace morphios {
namespace gui {

class Window : public CompositeWidget{
protected:
    bool dragging;

public:
    Window(Widget* parent, 
        common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h, 
        common::uint8_t r, common::uint8_t g, common::uint8_t b);
    ~Window();
    
    // Virtual overrides
    void onMouseDown(common::int32_t x, common:: int32_t y, common::uint8_t button);
    void onMouseUp(common::int32_t x, common:: int32_t y, common::uint8_t button);
    void onMouseMove(common::int32_t oldx, common:: int32_t oldy, common::int32_t newx, common:: int32_t newy);
};
    
} // namespace gui    
} // namespace morphios

#endif // WINDOW_H
