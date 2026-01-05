#ifndef UI_MANAGER
#define UI_MANAGER

#include <M5Unified.h>
#include "communication/constants.h"
#include "communication/bleManager.h"
#include "ui/constants.h"
#include "states.h"

void uiSetup();
void uiLoop();
void changeState(StateIndex idx);

#endif