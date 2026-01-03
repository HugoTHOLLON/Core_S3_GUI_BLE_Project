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
    // Reset, disable and clear all buttons
    for (int i = 0; i < buttonCount; i++)
    {
        if (buttons[i] != nullptr)
        {
            buttons[i]->reset();
            buttons[i]->setClickable(false);
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
        return index;

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
    : buttonNo(55, 165, 90, 40, "No"),
      buttonYes(170, 165, 90, 40, "Yes")
{
}

void StatePopup::enter(StateIndex previousState)
{
    this->previousState = previousState;
    switch (this->type)
    {
    case YesOrNo:
        buttonYes.x = 170;
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
}

void StateMenu::enter(StateIndex previousState)
{
    this->addButton(&buttonPrevious);
    this->addButton(&buttonNext);
    // call parent enter() which set the index to Same and reset and draw the buttons
    State::enter(previousState);
}

void StateMenu::exit(State *nextState)
{
    // call parent exit() which reset, disable and clear all buttons
    State::exit(nextState);
    // TODO pass things to next state if needed
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// State App /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

StateApp::StateApp()
{
}
