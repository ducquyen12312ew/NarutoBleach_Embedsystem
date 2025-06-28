#ifndef REWARDSCREENVIEW_HPP
#define REWARDSCREENVIEW_HPP

#include <gui_generated/rewardscreen_screen/RewardScreenViewBase.hpp>
#include <gui/rewardscreen_screen/RewardScreenPresenter.hpp>

class RewardScreenView : public RewardScreenViewBase
{
public:
    RewardScreenView();
    virtual ~RewardScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif
