#ifndef UI_MANAGER
#define UI_MANAGER

#include <M5Unified.h>
#include "communication/constants.h"
#include "communication/bleManager.h"
#include "ui/constants.h"
#include "states.h"

namespace UIManager
{
    // declare variables (they still need to be defined in uiManager.cpp)
    extern State *states[3]; // Tableau de pointeurs
    extern int currentStateIndex;

    void uiSetup();
    void uiLoop();
    void changeState(StateIndex idx);
    State *getState(StateIndex stateIdx);
}

#endif