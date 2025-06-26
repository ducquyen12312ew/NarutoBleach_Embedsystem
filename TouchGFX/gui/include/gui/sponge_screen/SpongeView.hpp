#ifndef SPONGEVIEW_HPP
#define SPONGEVIEW_HPP

#include <gui_generated/sponge_screen/SpongeViewBase.hpp>
#include <gui/sponge_screen/SpongePresenter.hpp>

class SpongeView : public SpongeViewBase
{
public:
    SpongeView();
    virtual ~SpongeView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleTickEvent();

protected:
    // Constants
    static const int RESET_TIME_MS = 2000; // 2 seconds to return to normal state

    // Timer system
    int resetTimer;
    bool resetTimerActive;

    // Menu system
    bool menuOpen;

    // GPIO and LED functions
    void initGPIO();
    void toggleLED();

    // Menu functions
    void hideActionButtons();
    void showActionButtons();
    void toggleActionMenu();

    // Training actions (no combat logic, just animations)
    void performAttack();
    void performDefend();
    void performSpecial();

    // Character state management
    void resetSpongeState();
    void startResetTimer();
};

#endif // SPONGEVIEW_HPP
