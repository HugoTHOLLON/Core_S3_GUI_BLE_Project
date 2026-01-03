#include "elements.h"

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
    static const int8_t gry = 40;
    return x > (this->x - gry) && y > (this->y - gry) && x < (this->x + this->width + gry) && y < (this->y + this->height + gry);
}

void ClickableArea::update() {}

Button::Button(int32_t x, int32_t y, int32_t width, int32_t height, const char *label)
    : ClickableArea(x, y, width, height)
{
    clickable = true;
    pressed = false;
    this->label = label;
}

bool Button::pressButton()
{
    if (isClickable())
    {
        Serial.println(("Button " + String(this->label) + " pressed").c_str());
        this->pressed = true;
        this->pressedTime = millis();
        this->draw();
        return true;
    }
    return false;
}

bool Button::isPressed() const { return pressed; }

bool Button::isClickable() const { return !pressed && clickable; }

void Button::setClickable(bool clickable) { this->clickable = clickable; }

void Button::reset()
{
    clickable = true;
    pressed = false;
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
    M5.Display.setTextSize(1.7);
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
