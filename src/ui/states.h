#ifndef UI_STATES
#define UI_STATES

#include <M5Unified.h>
#include "ui/constants.h"
#include "elements.h"
#include "utils/events.h"

enum class StateIndex
{
    Same = -1, // for the update return, when no state change is asked
    Menu = 0,
    Popup = 1,
    App = 2,

    COUNT // no values because it is used to calculate how many states there are
};

inline int index(StateIndex s) { return static_cast<int>(s); }
inline StateIndex indexToState(int i) { return (i >= 0 && i < static_cast<int>(StateIndex::COUNT)) ? static_cast<StateIndex>(i) : StateIndex::Same; }

class State
{
protected:
    // all of the clickable areas used by the state /!\ the order in which
    // the clickable areas are added matters: in case of a click if 2 areas are
    // overlapping, the one with the smallest index will be chosen first/!\.
    ClickableArea *clickableAreas[15];
    Signal<>::Handle *btnsPressedHandle[15];
    int areaCount;
    StateIndex index;

    void addArea(ClickableArea *area);
    void clearAreas();

    StateIndex stateType;

public:
    State();
    virtual void enter(StateIndex previousState);
    virtual void exit(State *nextState);
    virtual void checkIfElementPressed(m5::touch_detail_t clickDetails);
    virtual StateIndex update();
    StateIndex getStateType() const { return stateType; }
    // Virtual destructor for polymorphic base class
    virtual ~State() = default;
};

class StatePopup : public State
{
private:
    StateIndex previousState;

    Button buttonYes;
    Button buttonNo;
    Signal<>::Handle onBtnYesPressHdl;
    Signal<>::Handle onBtnNoHdl;
    void onButtonYesPressed();
    void onButtonNoPressed();

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
    Icon iconBLEApp;
    Icon iconWifiApp;
    Icon iconMQTTApp;
    Signal<>::Handle onBtnNextPressHdl;
    Signal<>::Handle onBtnPreviousHdl;
    void onButtonNextPressed();
    void onButtonPreviousPressed();

public:
    StateMenu();
    void enter(StateIndex previousState) override;
    void exit(State *nextState) override;
};

class StateApp : public State
{
public:
    StateApp();
};

#endif