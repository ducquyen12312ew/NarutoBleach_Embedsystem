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
    static const int RESET_TIME_MS = 2000;

    int resetTimer;
    bool resetTimerActive;

    bool menuOpen;

    void initGPIO();
    void toggleLED();

    void hideActionButtons();
    void showActionButtons();
    void toggleActionMenu();

    void performAttack();
    void performDefend();
    void performSpecial();

    void resetSpongeState();
    void startResetTimer();
};

#endif
