/*
    desktop.h - MorphiOS desktop library header file
*/

#ifndef DESKTOP_H
#define DESKTOP_H

#include "drivers/mouse.h"
#include "gui/widget.h"

namespace morphios {
namespace gui {

class Desktop : public gui::CompositeWidget, public drivers::MouseEventHandler {
protected:
    common::uint32_t mouseX;
    common::uint32_t mouseY;
public:
    Desktop(common::int32_t w, common::int32_t h, 
        common::uint8_t r, common::uint8_t g, common::uint8_t b);
    ~Desktop();

    void draw(common::graphicsContext *gc);
    void onMouseDown(common::uint8_t button);
    void onMouseUp(common::uint8_t button);
    void onMouseMove(common::int32_t xOffset, morphios::common::int32_t yOffset);
};

} // namespace gui    
} // namespace morphios

#endif // DESKTOP_H
