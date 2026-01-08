#ifndef DRAW_ICONS_H
#define DRAW_ICONS_H

#include <M5Unified.h>
#include "ui/constants.h"

enum class DrawableIcon
{
    NOTHING,
    BLE,
    WIFI,
    MQTT,
    HOME
};

void drawIcon(DrawableIcon icon, int32_t x, int32_t y, int32_t maxWidth, int32_t maxHeight);

#endif