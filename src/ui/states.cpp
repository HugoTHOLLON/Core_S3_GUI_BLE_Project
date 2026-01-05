#include "states.h"

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// State ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

State::State() : buttonCount(0)
{
    for (int i = 0; i < 10; i++)
        buttons[i] = nullptr;
}

void State::addButton(Button *btn)
{
    if (buttonCount < 10)
        buttons[buttonCount++] = btn;
}

void State::clearButtons()
{
    buttonCount = 0;
    for (int i = 0; i < 10; i++)
        buttons[i] = nullptr;
}

void State::enter(StateIndex previousState)
{
    this->index = StateIndex::Same;
    // Reset and draw all buttons
    for (int i = 0; i < buttonCount; i++)
    {
        if (buttons[i] != nullptr)
        {
            buttons[i]->reset();
            buttons[i]->draw();
        }
    }
}

void State::exit(State *nextState)
{
    // Reset, disable and clear all buttons plus disconnect all signals
    for (int i = 0; i < buttonCount; i++)
    {
        if (buttons[i] != nullptr)
        {
            buttons[i]->reset();
            buttons[i]->setClickable(false);
            buttons[i]->onPressed.disconnectAll();
        }
    }
    // clear buttons only removes the pointer in the array. The buttons are still in memory
    clearButtons();
}

void State::checkIfElementPressed(m5::touch_detail_t clickDetails)
{
    // Check all registered buttons
    for (int i = 0; i < buttonCount; i++)
    {
        if (buttons[i] != nullptr && buttons[i]->isInsideArea(clickDetails.x, clickDetails.y))
        {
            // If the button is pressed leave because only one button can be pressed at a time
            if (buttons[i]->pressButton())
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

    // Update all buttons
    for (int i = 0; i < buttonCount; i++)
    {
        if (buttons[i] != nullptr)
        {
            buttons[i]->update();
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

void StatePopup::enter(StateIndex previousState)
{
    this->previousState = previousState;
    // drawing state borders
    int32_t width = M5.Display.width() - 20;
    int32_t height = M5.Display.height() - 20 - statusBarHeight;
    int32_t y = statusBarHeight + 10;

    M5.Display.fillRect(7, y - 3, width + 6, height + 6, BUTTON_BG_COLOR);
    M5.Display.fillRect(10, y, width, height, BG_COLOR);

    int32_t textX = M5.Display.width() / 2;
    M5.Display.setTextSize(2);
    M5.Display.drawCenterString(this->title, textX, y + 10);
    int32_t msgY = y + M5.Display.fontHeight() + 20;
    M5.Display.setTextSize(1.6);
    M5.Display.drawCenterString(this->msg, textX, msgY);
    // button setup
    switch (this->type)
    {
    case YesOrNo:
        buttonYes.x = 180;
        this->addButton(&buttonYes);
        this->addButton(&buttonNo);
        break;

    case Confirmation:
    default:
        buttonYes.x = (M5.Display.width() / 2) - (buttonYes.width / 2);
        this->addButton(&buttonYes);
        break;
    }
    // call parent enter() which set the index to Same and reset and draw the buttons
    State::enter(previousState);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// State Menu ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

StateMenu::StateMenu()
    : buttonPrevious(15, 185, 130, 40, "Previous"),
      buttonNext(170, 185, 130, 40, "Next")
{
    stateType = StateIndex::Menu;
}

void StateMenu::enter(StateIndex previousState)
{
    // setup buttons
    this->addButton(&buttonPrevious);
    this->addButton(&buttonNext);
    if (!onBtnNextPressHdl.isValid())
        onBtnNextPressHdl = buttonNext.onPressed.connect([this]()
                                                         { onButtonNextPressed(); });
    if (!onBtnPreviousHdl.isValid())
        onBtnPreviousHdl = buttonPrevious.onPressed.connect([this]()
                                                            { onButtonPreviousPressed(); });
    // call parent enter() which set the index to Same and reset and draw the buttons
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

    // call parent exit() which reset, disable and clear all buttons and disconnect signals
    State::exit(nextState);
}

void StateMenu::onButtonNextPressed()
{
}

void StateMenu::onButtonPreviousPressed()
{
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// State App /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

StateApp::StateApp()
{
}
