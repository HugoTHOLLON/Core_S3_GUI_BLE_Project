#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <M5Unified.h>
#include <NimBLEDevice.h>

#define BLE_DEVICE_NAME "Hugo's really cool BLE device"
#define CUSTOM_BLE_SERVICE "1af37de5-470a-469d-a808-634fc1564aad"
#define CUSTOM_BLE_CHARACTERISTIC "ba75132a-eb84-4ff7-9a19-4868384ae0e5"

boolean deviceConnected();
void initBLE();
void updateBLEStatus();
void stopBLE();

extern NimBLEServer *pServer;

#endif