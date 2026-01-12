#ifndef APPS_H
#define APPS_H

#include <M5Unified.h>
#include "elements.h"
#include "utils/events.h"
#include "communication/bleManager.h"

enum class AppID
{
    BLE,
    WIFI,
    MQTT
};

class App
{
protected:
    int currentPage;
    int nbPages;

public:
    const char *name;

    App();
    App(const char *appName, int pages);
    int getCurrentPage();
    virtual bool nextPage();
    virtual bool previousPage();

    virtual void update();
    virtual void draw();
    virtual void checkIfElementPressed(m5::touch_detail_t clickDetails);
    // Virtual destructor for polymorphic base class
    virtual ~App() = default;
};

class AppBLE : public App
{
public:
    AppBLE();
};

class AppWIFI : public App
{
};

class AppMQTT : public App
{
};

#endif