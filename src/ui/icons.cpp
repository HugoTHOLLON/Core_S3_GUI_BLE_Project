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
    int cx = x + maxWidth / 2;
    int cy = y + maxHeight / 2;
    int r = maxWidth / 3;
    const int color = BLUE;

    // Draw WiFi waves
    M5.Display.fillArc(cx, cy, r, r - 2, 220, 320, color);
    M5.Display.fillArc(cx, cy, r * 2 / 3, r * 2 / 3 - 2, 220, 320, color);
    M5.Display.fillArc(cx, cy, r / 3, r / 3 - 2, 220, 320, color);
    M5.Display.fillCircle(cx, cy + r / 2, 2, color);
}

void drawMQTT(int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight)
{

    const int32_t logoWidth = min(maxWidth, maxHeight);
    int32_t rcx = x + ((maxWidth - logoWidth) / 2);  // rectangle top corner x
    int32_t rcy = y + ((maxHeight - logoWidth) / 2); // rectangle left corner y
    int32_t bly = rcy + logoWidth;                   // bottom left y
    // radius = distance from coordonates
    int32_t rsa = logoWidth * 0.50; // radius of small arc
    int32_t rma = logoWidth * 0.75; // radius of middle arc
    int32_t rba = logoWidth;        // radius of biggest arc
    int32_t at = 4;                 // arc thickness (how wide is the arc)
    const int color = APP_ICON_BG_COLOR;

    M5.Display.fillRoundRect(rcx, rcy, logoWidth, logoWidth, 3, MQTT_VIOLET);
    M5.Display.fillArc(rcx, bly, rsa, rsa - at, 270, 0, color);
    M5.Display.fillArc(rcx, bly, rma, rma - at, 270, 0, color);
    M5.Display.fillArc(rcx, bly, rba, rba - at, 270, 0, color);
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
