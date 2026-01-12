#include "states.h"

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

State::State() : areaCount(0)
{
    for (int i = 0; i < 10; i++)
        clickableAreas[i] = nullptr;
}

void State::addArea(ClickableArea *area)
{
    if (areaCount < 10)
        clickableAreas[areaCount++] = area;
}

void State::clearAreas()
{
    areaCount = 0;
    for (int i = 0; i < 10; i++)
        clickableAreas[i] = nullptr;
}

void State::enter(StateIndex previousState)
{
    this->index = StateIndex::Same;
    // Reset and draw all clickableAreas
    for (int i = 0; i < areaCount; i++)
    {
        if (clickableAreas[i] != nullptr)
        {
            clickableAreas[i]->reset();
            clickableAreas[i]->draw();
        }
    }
}

void State::exit(State *nextState)
{
    // Reset, disable and clear all clickableAreas plus disconnect all signals
    for (int i = 0; i < areaCount; i++)
    {
        if (clickableAreas[i] != nullptr)
        {
            clickableAreas[i]->reset();
            clickableAreas[i]->setClickable(false);
            clickableAreas[i]->onPressed.disconnectAll();
        }
    }
    // clear clickableAreas only removes the pointer in the array. The clickableAreas are still in memory
    clearAreas();
}

void State::checkIfElementPressed(m5::touch_detail_t clickDetails)
{
    // Check all registered clickableAreas
    for (int i = 0; i < areaCount; i++)
    {
        if (clickableAreas[i] != nullptr && clickableAreas[i]->isInsideArea(clickDetails.x, clickDetails.y))
        {
            // If the button is pressed leave because only one button can be pressed at a time
            if (clickableAreas[i]->press())
                return;
        }
    }
}

StateIndex State::update()
{
    // if the index is not Same it means the State must change next loop for the new State in the index
    if (index != StateIndex::Same)
    {
        Serial.println("Index is different");
        return index;
    }

    // Update all clickableAreas
    for (int i = 0; i < areaCount; i++)
    {
        if (clickableAreas[i] != nullptr)
        {
            clickableAreas[i]->update();
        }
    }

    return StateIndex::Same;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// State Popup ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

StatePopup::StatePopup()
    : buttonNo(45, 180, 90, 30, "No", 1.5),
      buttonYes(180, 180, 90, 30, "Yes", 1.5)
{
    stateType = StateIndex::Popup;
}

void StatePopup::setPopup(PopupType type, const char *title, const char *msg)
{
    this->type = type;
    this->title = title;
    this->msg = msg;
}

void StatePopup::draw()
{
    // setup variables
    int32_t width = M5.Display.width() - 20;
    int32_t height = M5.Display.height() - 20 - statusBarHeight;
    int32_t y = statusBarHeight + 10;

    // drawing state borders
    M5.Display.fillRect(7, y - 3, width + 6, height + 6, BUTTON_BG_COLOR);
    M5.Display.fillRect(10, y, width, height, BG_COLOR);

    // draw title
    int32_t textX = M5.Display.width() / 2;
    M5.Display.setTextSize(2);
    M5.Display.drawCenterString(this->title, textX, y + 10);

    // draw message
    int32_t msgY = y + M5.Display.fontHeight() + 20;
    M5.Display.setTextSize(1.6);
    M5.Display.drawCenterString(this->msg, textX, msgY);
}

void StatePopup::enter(StateIndex previousState)
{
    this->previousState = previousState;

    this->draw();

    // button setup
    switch (this->type)
    {
    case YesOrNo:
        buttonYes.x = 180;
        this->addArea(&buttonYes);
        this->addArea(&buttonNo);
        if (!onBtnYesPressHdl.isValid())
            onBtnYesPressHdl = buttonYes.onPressed.connect([this]()
                                                           { onButtonYesPressed(); });
        if (!onBtnNoHdl.isValid())
            onBtnNoHdl = buttonNo.onPressed.connect([this]()
                                                    { onButtonNoPressed(); });
        break;

    case Confirmation:
    default:
        buttonYes.x = (M5.Display.width() / 2) - (buttonYes.width / 2);
        this->addArea(&buttonYes);
        if (!onBtnYesPressHdl.isValid())
            onBtnYesPressHdl = buttonYes.onPressed.connect([this]()
                                                           { onButtonYesPressed(); });
        break;
        this->type = type;
        this->title = title;
        this->msg = msg;
    }

    // call parent enter() which set the index to Same and reset and draw the clickableAreas
    State::enter(previousState);
}

void StatePopup::onButtonYesPressed() { this->index = this->previousState; }
void StatePopup::onButtonNoPressed() { this->index = this->previousState; }

Button StatePopup::getButtonYes() { return this->buttonYes; }
Button StatePopup::getButtonNo() { return this->buttonNo; }

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// State Menu ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

StateMenu::StateMenu()
    : iconBLEApp(15, 80, 90, 90, "BLE", DrawableIcon::BLE),
      iconWifiApp(115, 80, 90, 90, "Wifi", DrawableIcon::WIFI),
      iconMQTTApp(215, 80, 90, 90, "MQTT", DrawableIcon::MQTT)
{
    stateType = StateIndex::Menu;
}

void StateMenu::enter(StateIndex previousState)
{
    // setup clickableAreas
    this->addArea(&iconBLEApp);
    this->addArea(&iconWifiApp);
    this->addArea(&iconMQTTApp);

    if (!onIconBLEPressedHdl.isValid())
        onIconBLEPressedHdl = iconBLEApp.onPressed.connect([this]()
                                                           { onIconBLEPressed(); });

    // call parent enter() which set the index to Same and reset and draw the clickableAreas
    State::enter(previousState);
}

void StateMenu::exit(State *nextState)
{
    // Pass informations to popup
    switch (nextState->getStateType())
    {
    case StateIndex::Popup:
        // It IS a StatePopup - you can access popup-specific members
        StatePopup *popup = static_cast<StatePopup *>(nextState);
        popup->type = StatePopup::YesOrNo;
        popup->title = "Confirm Exit";
        popup->msg = "Are you sure?";
        break;
    }

    // call parent exit() which reset, disable and clear all clickableAreas and disconnect signals
    State::exit(nextState);
}

void StateMenu::onIconBLEPressed() { this->index = StateIndex::App; }

Icon StateMenu::getIconBLEApp() { return this->iconBLEApp; }
Icon StateMenu::getIconWifiApp() { return this->iconWifiApp; }
Icon StateMenu::getIconMQTTApp() { return this->iconMQTTApp; }

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// State App /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

StateApp::StateApp()
    : iconHome(5, 25, 60, 50, "Home", DrawableIcon::HOME),
      buttonPrev(15, 190, 130, 40, "Previous", 1.6),
      buttonNext(170, 190, 130, 40, "Next", 1.6),
      appBLE(),
      appWIFI(),
      appMQTT()
{
    stateType = StateIndex::App;
    this->currentApp = &appBLE;
}

void StateApp::setApp(AppID id)
{
    switch (id)
    {
    case AppID::BLE:
        currentApp = &appBLE;
        break;
    case AppID::WIFI:
        currentApp = &appWIFI;
        break;
    case AppID::MQTT:
        currentApp = &appMQTT;
        break;
    }
}

void StateApp::enter(StateIndex previousState)
{
    this->addArea(&buttonPrev);
    this->addArea(&buttonNext);
    this->addArea(&iconHome);

    if (!onIconHomePressHdl.isValid())
        onIconHomePressHdl = iconHome.onPressed.connect([this]()
                                                        { onIconHomePressed(); });
    if (!onBtnNextPressHdl.isValid())
        onBtnNextPressHdl = buttonNext.onPressed.connect([this]()
                                                         { onButtonNextPressed(); });
    if (!onBtnPrevPressHdl.isValid())
        onBtnPrevPressHdl = buttonPrev.onPressed.connect([this]()
                                                         { onButtonPrevPressed(); });

    State::enter(previousState);

    currentApp->draw();
}

void StateApp::onIconHomePressed() { this->index = StateIndex::Menu; }
void StateApp::onButtonNextPressed() { currentApp->nextPage(); }
void StateApp::onButtonPrevPressed() { currentApp->previousPage(); }

Icon StateApp::getIconHome() { return this->iconHome; }
Button StateApp::getButtonNext() { return this->buttonNext; }
Button StateApp::getButtonPrev() { return this->buttonPrev; }