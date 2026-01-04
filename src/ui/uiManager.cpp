#include "uiManager.h"

State *states[10]; // Tableau de pointeurs
int currentStateIndex;
int stateCount;

void uiSetup()
{
    Serial.println("------------------- UI Setup started");
    states[index(StateIndex::Menu)] = new StateMenu();
    states[index(StateIndex::Popup)] = new StatePopup();
    states[index(StateIndex::App)] = new StateApp();
    currentStateIndex = index(StateIndex::Menu);

    Serial.println("------------------- UI drawing status bar");
    drawStatusBar(ConnexionStatus::DISCONNECTED, ConnexionStatus::CONNECTED, ConnexionStatus::PENDING);

    Serial.println("------------------- UI entering state");
    states[currentStateIndex]->enter(StateIndex::Same);
    Serial.println("------------------- UI state entered");
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
        Serial.println("StateIndex different - Changing state");
        int oldState = currentStateIndex;
        currentStateIndex = index(idx);
        states[oldState]->exit(states[currentStateIndex]);
        M5.Display.fillScreen(BG_COLOR);
        drawStatusBar(ConnexionStatus::CONNECTED, ConnexionStatus::CONNECTED, ConnexionStatus::DISCONNECTED);
        states[currentStateIndex]->enter(indexToState(oldState));
    }
}