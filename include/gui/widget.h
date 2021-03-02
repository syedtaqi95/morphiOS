/*
    widget.h - MorphiOS GUI widget header file
*/

#ifndef WIDGET_H
#define WIDGET_H

#include "common/types.h"

namespace morphios {
namespace gui {

class Widget {
protected:
    Widget *parent; // Parent widget

    // Positional members
    common::int32_t x;
    common::int32_t y;
    common::int32_t w; // Width
    common::int32_t h; // Height

    // Colour related members
    common::uint8_t r;
    common::uint8_t g;
    common::uint8_t b;

public:
    Widget(common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h);
    ~Widget();
    void draw();
};
    
} // namespace gui    
} // namespace morphios

#endif // WIDGET_H
