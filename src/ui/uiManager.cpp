#include "uiManager.h"

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Status Bar /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

// these variables will be updated later in the code
int32_t statusBarHeight = 0;
int32_t indicatorBaseXPosition = 0;
int32_t indicatorWifiX = 0;
int32_t indicatorBLEX = 0;
int32_t indicatorMQTTX = 0;

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
    // associated text. The computed position is complicated for nothing (just to look good).
    indicatorBaseXPosition = M5.Display.width() - STATUS_BAR_PADDING + (M5.Display.fontWidth() / 2);
    indicatorWifiX = indicatorBaseXPosition - M5.Display.textWidth("  Wifi   BLE   MQTT");
    indicatorBLEX = indicatorBaseXPosition - M5.Display.textWidth("  BLE   MQTT");
    indicatorMQTTX = indicatorBaseXPosition - M5.Display.textWidth("  MQTT");
    drawStatusIndicator(wifiStatus, indicatorWifiX);
    drawStatusIndicator(bleStatus, indicatorBLEX);
    drawStatusIndicator(mqttStatus, indicatorMQTTX);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Event handling ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void onWifiConnexionStatusUpdated(ConnexionStatus wifiStatus)
{
    drawStatusIndicator(wifiStatus, indicatorWifiX);
}

void onBLEConnexionStatusUpdated(ConnexionStatus bleStatus)
{
    drawStatusIndicator(bleStatus, indicatorBLEX);
}

void onMQTTConnexionStatusUpdated(ConnexionStatus mqttStatus)
{
    drawStatusIndicator(mqttStatus, indicatorMQTTX);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// States Management /////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

namespace UIManager
{
    State *states[3]; // Tableau de pointeurs
    int currentStateIndex;

    State *getState(StateIndex stateIdx)
    {
        return states[index(stateIdx)];
    }

    void changeState(StateIndex idx)
    {
        Serial.println("Changing state");
        int oldState = currentStateIndex;
        currentStateIndex = index(idx);
        states[oldState]->exit(states[currentStateIndex]);
        M5.Display.fillScreen(BG_COLOR);
        states[currentStateIndex]->enter(indexToState(oldState));
        drawStatusBar(ConnexionStatus::DISCONNECTED, BLEManager::getConnexionStatus(), ConnexionStatus::PENDING);
    }
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Setup and Loop ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

namespace UIManager
{
    void uiSetup()
    {
        Serial.println("-------- UI Setup started");
        states[index(StateIndex::Menu)] = new StateMenu();
        states[index(StateIndex::Popup)] = new StatePopup();
        states[index(StateIndex::App)] = new StateApp();
        currentStateIndex = index(StateIndex::Menu);

        Serial.println("-------- UI drawing status bar");
        drawStatusBar(ConnexionStatus::DISCONNECTED, BLEManager::getConnexionStatus(), ConnexionStatus::PENDING);

        Serial.println("-------- UI connecting connexion status updated signals");
        BLEManager::connStatusUpdated.connect(onBLEConnexionStatusUpdated);

        Serial.println("-------- UI entering state Menu");
        states[currentStateIndex]->enter(StateIndex::Same);
    }

    void uiLoop()
    {
        M5.update();
        auto t = M5.Touch.getDetail();
        if (t.wasPressed())
        {
            states[currentStateIndex]->checkIfElementPressed(t);
        }
        // update various elements
        StateIndex idx = states[currentStateIndex]->update();
        if (idx != StateIndex::Same)
        {
            Serial.print("StateIndex different - ");
            changeState(idx);
        }
    }
}