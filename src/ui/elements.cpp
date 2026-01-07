#include "elements.h"

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Clickable Area ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

ClickableArea::ClickableArea(int32_t x, int32_t y, int32_t width, int32_t height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

bool ClickableArea::isInsideArea(int32_t x, int32_t y)
{
    // area generosity (bigger hitbox to take into accounts clicks really close to area but not exactly in it)
    static const int8_t gry = 10;
    return x > (this->x - gry) && y > (this->y - gry) && x < (this->x + this->width + gry) && y < (this->y + this->height + gry);
}

void ClickableArea::update() {}

void ClickableArea::draw() {}

bool ClickableArea::press()
{
    if (isClickable())
    {
        Serial.println("ClickableArea pressed");
        this->pressed = true;
        this->draw();
        this->onPressed.emit();
        return true;
    }
    return false;
}

bool ClickableArea::isPressed() const { return pressed; }

bool ClickableArea::isClickable() const { return !pressed && clickable; }

void ClickableArea::setClickable(bool clickable) { this->clickable = clickable; }

void ClickableArea::reset()
{
    clickable = true;
    pressed = false;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// Button ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

Button::Button(int32_t x, int32_t y, int32_t width, int32_t height, const char *label)
    : Button(x, y, width, height, label, 1.7)
{
}

Button::Button(int32_t x, int32_t y, int32_t width, int32_t height, const char *label, float lblSize)
    : ClickableArea(x, y, width, height)
{
    clickable = true;
    pressed = false;
    this->label = label;
    this->labelSize = lblSize;
}

bool Button::press()
{
    // call parent press()
    if (ClickableArea::press())
    {
        Serial.println(("Button " + String(this->label) + " pressed").c_str());
        this->pressedTime = millis();
        return true;
    }
    return false;
}

void Button::draw()
{
    int32_t fgX = pressed ? x : x - 5;
    int32_t fgY = pressed ? y : y - 3;

    // Reset rectangle
    if (pressed)
        M5.Display.fillRect(x - 5, y - 3, width, height, BG_COLOR);

    M5.Display.fillRect(x, y, width, height, BUTTON_BG_COLOR);
    M5.Display.fillRect(fgX, fgY, width, height, BUTTON_COLOR);

    textdatum_t previousDatum = M5.Display.getTextDatum();
    M5.Display.setTextDatum(MC_DATUM);
    M5.Display.setTextSize(this->labelSize);
    M5.Display.setTextColor(TEXT_COLOR);
    M5.Display.drawString(label, fgX + (width / 2), fgY + (height / 2));

    M5.Display.setTextDatum(previousDatum);
}

void Button::update()
{
    // 300 ms
    if (isPressed() && (millis() - pressedTime >= 300))
    {
        pressed = false;
        draw();
    }
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// Icon ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

Icon::Icon(int32_t x, int32_t y, int32_t width, int32_t height, const char *label)
    : Icon(x, y, width, height, label, DrawableIcon::NOTHING)
{
}

Icon::Icon(int32_t x, int32_t y, int32_t width, int32_t height, const char *label, DrawableIcon icon)
    : ClickableArea(x, y, width, height)
{
    clickable = true;
    pressed = false;
    this->label = label;
    this->icon = icon;
}

bool Icon::press()
{
    // call parent press()
    if (ClickableArea::press())
    {
        Serial.println(("Icon " + String(this->label) + " pressed").c_str());
        this->pressedTime = millis();
        return true;
    }
    return false;
}

void Icon::draw()
{
    if (pressed)
        M5.Display.fillRect(x, y, width, height, APP_ICON_PRESSED_BG_COLOR);
    else
        M5.Display.fillRect(x, y, width, height, APP_ICON_BG_COLOR);

    textdatum_t previousDatum = M5.Display.getTextDatum();
    M5.Display.setTextDatum(BC_DATUM);
    M5.Display.setTextSize(1.6);
    M5.Display.setTextColor(TEXT_COLOR);
    M5.Display.drawString(label, x + (width / 2), y + height - 5);
    drawIcon(this->icon, x + 3, y + 7, width - 6, height - 15 - M5.Display.fontHeight());

    M5.Display.setTextDatum(previousDatum);
}

void Icon::update()
{
    // 300 ms
    if (isPressed() && (millis() - pressedTime >= 300))
    {
        pressed = false;
        this->draw();
    }
}