#ifndef UI_STATES
#define UI_STATES

#include <M5Unified.h>
#include "ui/constants.h"
#include "elements.h"

enum class StateIndex
{
    Same = -1, // for the update return, when no state change is asked
    Menu = 0,
    PopupConf = 1, // Popup Confirmation
    PopupYesNo = 2,
    App = 3,
};

inline int index(StateIndex s) { return static_cast<int>(s); }

class State
{
private:
    StateIndex index;

public:
    virtual void enter(StateIndex previousState);
    virtual void exit(State *nextState);
    virtual void checkIfElementPressed(m5::touch_detail_t clickDetails);
    virtual StateIndex update();
    // Virtual destructor for polymorphic base class
    virtual ~State() = default;
};

class StatePopup : public State
{
private:
    StateIndex previousState;
    Button buttonYes;
    Button buttonNo;

public:
    enum PopupType
    {
        Confirmation,
        YesOrNo
    };

    PopupType type = Confirmation;
    const char *title = "";
    const char *msg = "";

    StatePopup();
    void enter(StateIndex previousState) override;
};

class StateMenu : public State
{
private:
    Button buttonNext;
    Button buttonPrevious;
    StateIndex index;

public:
    StateMenu();
    void enter(StateIndex previousState) override;
    void exit(State *nextState) override;
    void checkIfElementPressed(m5::touch_detail_t clickDetails) override;
    StateIndex update() override;
};

class StateApp : public State
{
public:
    StateApp();
};

#endif