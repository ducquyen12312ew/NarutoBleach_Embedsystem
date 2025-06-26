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

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~RewardScreenPresenter() {}

private:
    RewardScreenPresenter();

    RewardScreenView& view;
};

#endif // REWARDSCREENPRESENTER_HPP
