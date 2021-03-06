/*
    render.h - Rendering class
*/

#ifndef RENDER_H
#define RENDER_H

#include "drivers/vga.h"
#include "common/graphicscontext.h"
#include "common.h"

namespace morphios{
namespace gui{

// Renders the frame prior to moving it to video memory
class Render : public drivers::VGA{
private:
    struct Pixel {
        common::uint8_t r;
        common::uint8_t g;
        common::uint8_t b;
    };
    // TODO : make this dynamically sized
    Pixel frameBuffer[320][200];

public:
    Render();
    ~Render();
    void display(common::graphicsContext* gc);
    void putPixel(common::int32_t x, common::int32_t y, common::uint8_t r, common::uint8_t g, common::uint8_t b);
};

}
}

#endif // RENDER_H
