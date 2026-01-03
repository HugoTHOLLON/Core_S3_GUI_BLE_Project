#ifndef DRAW_UI_ELEMENT
#define DRAW_UI_ELEMENT

#include <M5Unified.h>
#include "communication/constants.h"
#include "ui/constants.h"

void drawStatusBar(ConnexionStatus wifiStatus, ConnexionStatus bleStatus, ConnexionStatus mqttStatus);

#endif