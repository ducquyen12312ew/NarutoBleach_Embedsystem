#ifndef SAKURAFINALSCREENPRESENTER_HPP
#define SAKURAFINALSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SakuraFinalScreenView;

class SakuraFinalScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SakuraFinalScreenPresenter(SakuraFinalScreenView& v);

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

    virtual ~SakuraFinalScreenPresenter() {}

private:
    SakuraFinalScreenPresenter();

    SakuraFinalScreenView& view;
};

#endif // SAKURAFINALSCREENPRESENTER_HPP
