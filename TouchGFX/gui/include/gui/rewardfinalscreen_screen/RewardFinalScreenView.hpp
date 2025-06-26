#ifndef REWARDFINALSCREENVIEW_HPP
#define REWARDFINALSCREENVIEW_HPP

#include <gui_generated/rewardfinalscreen_screen/RewardFinalScreenViewBase.hpp>
#include <gui/rewardfinalscreen_screen/RewardFinalScreenPresenter.hpp>

class RewardFinalScreenView : public RewardFinalScreenViewBase
{
public:
    RewardFinalScreenView();
    virtual ~RewardFinalScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // REWARDFINALSCREENVIEW_HPP
