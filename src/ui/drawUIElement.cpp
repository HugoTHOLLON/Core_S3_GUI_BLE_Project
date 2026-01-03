#include "drawUIElement.h"

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Status Bar /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

int32_t statusBarHeight = 0;

void drawStatusIndicator(ConnexionStatus status, int32_t x)
{
    int color;

    switch (status)
    {
    case ConnexionStatus::CONNECTED:
        color = CONNECTED_STATUS_COLOR;
        break;
    case ConnexionStatus::PENDING:
        color = PENDING_STATUS_COLOR;
        break;
    case ConnexionStatus::DISCONNECTED:
    default:
        color = DISCONNECTED_STATUS_COLOR;
        break;
    }

    M5.Display.fillCircle(x, statusBarHeight / 2, M5.Display.fontHeight() / 2, color);
}

void drawStatusBar(ConnexionStatus wifiStatus, ConnexionStatus bleStatus, ConnexionStatus mqttStatus)
{
    const int32_t screenWidth = M5.Display.width();

    // setting text size and drawing bar based on the text height + padding
    M5.Display.setTextSize(1.4);
    M5.Display.setTextColor(TEXT_COLOR);
    statusBarHeight = M5.Display.fontHeight() + STATUS_BAR_PADDING * 2;
    M5.Display.fillRect(0, 0, screenWidth, statusBarHeight, STATUS_BAR_BG_COLOR);

    // drawing text on this corner
    M5.Display.drawRightString("Wifi   BLE   MQTT", screenWidth - STATUS_BAR_PADDING, STATUS_BAR_PADDING);
    // drawing circles to indicate the status. The circle is placed on the left of it's
    // associated text. position is here to place it so it looks good (complicated for nothing).
    int32_t position = screenWidth - STATUS_BAR_PADDING + (M5.Display.fontWidth() / 2);
    drawStatusIndicator(wifiStatus, position - M5.Display.textWidth("  Wifi   BLE   MQTT"));
    drawStatusIndicator(bleStatus, position - M5.Display.textWidth("  BLE   MQTT"));
    drawStatusIndicator(mqttStatus, position - M5.Display.textWidth("  MQTT"));
}
