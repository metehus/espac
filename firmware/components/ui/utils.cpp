#include "utils.h"

uint32_t dim_color(uint32_t color, float factor) {
    factor = std::max(0.0f, std::min(1.0f, factor));

    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    r = static_cast<uint8_t>(r * factor);
    g = static_cast<uint8_t>(g * factor);
    b = static_cast<uint8_t>(b * factor);

    return (color & 0xFF000000) | (r << 16) | (g << 8) | b;
}