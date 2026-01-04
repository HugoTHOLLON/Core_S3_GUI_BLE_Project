#ifndef UI_ELEMENTS
#define UI_ELEMENTS

#include <M5Unified.h>
#include "ui/constants.h"
#include "utils/events.h"

class ClickableArea
{
public:
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;

    ClickableArea(int32_t x, int32_t y, int32_t width, int32_t height);
    bool isInsideArea(int32_t x, int32_t y);
    virtual void update();
};

class Button : public ClickableArea
{
private:
    bool clickable;
    bool pressed;
    unsigned long pressedTime;

public:
    const char *label;
    float labelSize;
    Signal<> onPressed;

    Button(int32_t x, int32_t y, int32_t width, int32_t height, const char *label);
    Button(int32_t x, int32_t y, int32_t width, int32_t height, const char *label, float lblSize);
    bool pressButton();
    bool isPressed() const;
    bool isClickable() const;
    void setClickable(bool clickable);
    void draw();
    void reset();
    void update() override;
};

#endif