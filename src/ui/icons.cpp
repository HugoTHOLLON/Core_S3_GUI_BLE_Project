#include "icons.h"

void drawBLE(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight)
{
    int cx = x + maxWidth / 2 - 3; // center x
    int cy = y + maxHeight / 2;    // center y
    int h = maxHeight;             // height
    const int color = BLUE;

    // Draw Bluetooth symbol (simplified)
    M5.Display.drawLine(cx, cy - h / 2, cx, cy + h / 2, color);         // vertical bar
    M5.Display.drawLine(cx, cy - h / 2, cx + h / 4, cy - h / 4, color); // top of upper B
    M5.Display.drawLine(cx + h / 4, cy - h / 4, cx, cy, color);         // bottom of upper B
    M5.Display.drawLine(cx, cy, cx + h / 4, cy + h / 4, color);         // top of lower B
    M5.Display.drawLine(cx + h / 4, cy + h / 4, cx, cy + h / 2, color); // bottom of lower B
}

void drawWIFI(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight)
{
}

void drawMQTT(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight)
{
}

void drawIcon(DrawableIcon icon, int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight)
{
    switch (icon)
    {
    case DrawableIcon::BLE:
        drawBLE(x, y, maxWidth, maxHeight);
        break;
    case DrawableIcon::WIFI:
        drawWIFI(x, y, maxWidth, maxHeight);
        break;
    case DrawableIcon::MQTT:
        drawMQTT(x, y, maxWidth, maxHeight);
        break;

    default:
        break;
    }
}
