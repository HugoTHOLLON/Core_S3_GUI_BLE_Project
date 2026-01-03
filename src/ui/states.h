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

    COUNT // no values because it is used to calculate how many states there are
};

inline int index(StateIndex s) { return static_cast<int>(s); }
inline StateIndex indexToState(int i) { return (i >= 0 && i < static_cast<int>(StateIndex::COUNT)) ? static_cast<StateIndex>(i) : StateIndex::Same; }

class State
{
protected:
    // all of the buttons used by the state
    // /!\ the order in which the buttons are added matters: in case of a click if 2 buttons are
    // overlapping, the one with the smallest index will be chosen first/!\.
    Button *buttons[10];
    int buttonCount;
    StateIndex index;

    void addButton(Button *btn);
    void clearButtons();

public:
    State();
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
};

class StateApp : public State
{
public:
    StateApp();
};

#endif