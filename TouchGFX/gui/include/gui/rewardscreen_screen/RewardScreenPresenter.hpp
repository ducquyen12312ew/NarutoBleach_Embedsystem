#ifndef REWARDSCREENPRESENTER_HPP
#define REWARDSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class RewardScreenView;

class RewardScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    RewardScreenPresenter(RewardScreenView& v);
    virtual void activate();
    virtual void deactivate();
    virtual ~RewardScreenPresenter() {}

private:
    RewardScreenPresenter();
    RewardScreenView& view;
};

#endif
