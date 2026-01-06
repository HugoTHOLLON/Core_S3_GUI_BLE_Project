#ifndef UI_CONSTANTS
#define UI_CONSTANTS

#include <M5Unified.h>

const int8_t STATUS_BAR_PADDING = 5;
extern int32_t statusBarHeight;

const int8_t VIEWPORT_PADDING = 15;

const int TEXT_COLOR = BLACK;
const int BG_COLOR = WHITE;
const int STATUS_BAR_BG_COLOR = TFT_LIGHTGRAY;
const int BUTTON_COLOR = TFT_LIGHTGRAY;
const int BUTTON_BG_COLOR = BLACK;

const int APP_ICON_BG_COLOR = 0xE71C; // really light lightgray
const int APP_ICON_PRESSED_BG_COLOR = DARKGREY;

const int CONNECTED_STATUS_COLOR = GREEN;
const int PENDING_STATUS_COLOR = ORANGE;
const int DISCONNECTED_STATUS_COLOR = RED;

#endif