#include "bleManager.h"

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Callbacks declaration ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

class ServerCallbacks : public NimBLEServerCallbacks
{
    void onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo) override;
    void onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, int reason) override;
} serverCallbacks;

class CharacteristicCallbacks : public NimBLECharacteristicCallbacks
{
    void onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) override;
    void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) override;
    /** The value returned in code is the NimBLE host return code. */
    void onStatus(NimBLECharacteristic *pCharacteristic, int code) override;
    /** Peer subscribed to notifications/indications */
    void onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue) override;
} chrCallbacks;

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Constants and Global Variables ///////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

namespace BLEManager
{
    /////////////////////////////// Private ////////////////////////
    // Internal variables (not exposed in header)
    NimBLEServer *pServer = nullptr;
    ConnexionStatus bleStatus = ConnexionStatus::DISCONNECTED;

    // private method not defined in header
    void setConnexionStatus(ConnexionStatus sts)
    {
        bleStatus = sts;
        connStatusUpdated.emit(sts);
    }

    /////////////////////////////// Public ////////////////////////.
    Signal<ConnexionStatus> connStatusUpdated;

    ConnexionStatus getConnexionStatus() { return bleStatus; }

    void init()
    {
        Serial.println("Setting up BLE");
        // initialize the library to prepare the NimBLE stack to be ready for commands
        NimBLEDevice::init(BLE_DEVICE_NAME);
        // define the device as a server (peripheral) and set callbacks
        pServer = NimBLEDevice::createServer();
        pServer->setCallbacks(&serverCallbacks);

        // create a new service with the given UUID (here it is a custom service)
        NimBLEService *pCustomService = pServer->createService(CUSTOM_BLE_SERVICE);

        // create and add a new characteristic to the custom service. By default a characteristic has READ and WRITE rights
        NimBLECharacteristic *pCharacteristic = pCustomService->createCharacteristic(CUSTOM_BLE_CHARACTERISTIC);
        pCharacteristic->setValue("LOL");
        pCharacteristic->setCallbacks(&chrCallbacks);

        Serial.println("-- Server, Service and Characteristic have been set up");

        pCustomService->start();
        Serial.println("-- Service started");

        // Start the server (optional but good practice)
        pServer->start();
        Serial.println("-- Server started");

        NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
        pAdvertising->setName(BLE_DEVICE_NAME);
        pAdvertising->addServiceUUID(CUSTOM_BLE_SERVICE);
        pAdvertising->start();
        Serial.println("-- Advertising started");
    }

    void update() {}

    void stop() { NimBLEDevice::deinit(true); }
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// Callbacks ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void ServerCallbacks::onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo)
{
    // Check if there is already a connexion to limit it to 1
    if (pServer->getConnectedCount() > 1)
    {
        pServer->disconnect(connInfo.getConnHandle());
        return;
    }
    Serial.println("Client connected");

    // Stop advertising to prevent additional connexions
    NimBLEDevice::getAdvertising()->stop();

    pServer->updateConnParams(connInfo.getConnHandle(), 24, 48, 0, 180);
    BLEManager::setConnexionStatus(ConnexionStatus::CONNECTED);
}

void ServerCallbacks::onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, int reason)
{
    // Only restart advertising if no connexions remain
    if (pServer->getConnectedCount() == 0)
    {
        Serial.println("Client disconnected - start advertising");
        NimBLEDevice::startAdvertising();
        BLEManager::setConnexionStatus(ConnexionStatus::DISCONNECTED);
    }
}

/** Handler class for characteristic actions */
void CharacteristicCallbacks::onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
{
    Serial.printf("%s : onRead(), value: %s\n",
                  pCharacteristic->getUUID().toString().c_str(),
                  pCharacteristic->getValue().c_str());
}

void CharacteristicCallbacks::onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
{
    Serial.printf("%s : onWrite(), value: %s\n",
                  pCharacteristic->getUUID().toString().c_str(),
                  pCharacteristic->getValue().c_str());
}

/**
 *  The value returned in code is the NimBLE host return code.
 */
void CharacteristicCallbacks::onStatus(NimBLECharacteristic *pCharacteristic, int code)
{
    Serial.printf("Notification/Indication return code: %d, %s\n", code, NimBLEUtils::returnCodeToString(code));
}

/** Peer subscribed to notifications/indications */
void CharacteristicCallbacks::onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue)
{
    std::string str = "Client ID: ";
    str += connInfo.getConnHandle();
    str += " Address: ";
    str += connInfo.getAddress().toString();
    if (subValue == 0)
    {
        str += " Unsubscribed to ";
    }
    else if (subValue == 1)
    {
        str += " Subscribed to notifications for ";
    }
    else if (subValue == 2)
    {
        str += " Subscribed to indications for ";
    }
    else if (subValue == 3)
    {
        str += " Subscribed to notifications and indications for ";
    }
    str += std::string(pCharacteristic->getUUID());

    Serial.printf("%s\n", str.c_str());
}
