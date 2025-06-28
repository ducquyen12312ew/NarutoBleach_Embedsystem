#ifndef REWARDFINALSCREENPRESENTER_HPP
#define REWARDFINALSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class RewardFinalScreenView;

class RewardFinalScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    RewardFinalScreenPresenter(RewardFinalScreenView& v);
    virtual void activate();
    virtual void deactivate();
    virtual ~RewardFinalScreenPresenter() {}

private:
    RewardFinalScreenPresenter();
    RewardFinalScreenView& view;
};

#endif
