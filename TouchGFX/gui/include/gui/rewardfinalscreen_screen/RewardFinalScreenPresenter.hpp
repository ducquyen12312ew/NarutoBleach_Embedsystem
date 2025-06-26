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

    virtual ~RewardFinalScreenPresenter() {}

private:
    RewardFinalScreenPresenter();

    RewardFinalScreenView& view;
};

#endif // REWARDFINALSCREENPRESENTER_HPP
