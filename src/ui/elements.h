#ifndef UI_ELEMENTS
#define UI_ELEMENTS

#include <M5Unified.h>
#include "ui/constants.h"
#include "utils/events.h"
#include "ui/icons.h"

class ClickableArea
{
protected:
    bool clickable;
    bool pressed;

public:
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
    Signal<> onPressed;

    ClickableArea(int32_t x, int32_t y, int32_t width, int32_t height);
    bool isInsideArea(int32_t x, int32_t y);

    virtual bool press();
    bool isPressed() const;
    bool isClickable() const;
    void setClickable(bool clickable);
    virtual void draw();
    virtual void reset();
    virtual void update();
};

class Button : public ClickableArea
{
private:
    unsigned long pressedTime;

public:
    const char *label;
    float labelSize;

    Button(int32_t x, int32_t y, int32_t width, int32_t height, const char *label);
    Button(int32_t x, int32_t y, int32_t width, int32_t height, const char *label, float lblSize);
    bool press() override;
    void draw() override;
    void update() override;
};

class Icon : public ClickableArea
{
private:
    unsigned long pressedTime;

public:
    const char *label;
    DrawableIcon icon;

    Icon(int32_t x, int32_t y, int32_t width, int32_t height, const char *label);
    Icon(int32_t x, int32_t y, int32_t width, int32_t height, const char *label, DrawableIcon icon);
    bool press() override;
    void draw() override;
    void update() override;
};

#endif