#ifndef SAKURAGAMESCREENPRESENTER_HPP
#define SAKURAGAMESCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SakuraGameScreenView;

class SakuraGameScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SakuraGameScreenPresenter(SakuraGameScreenView& v);

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

    virtual ~SakuraGameScreenPresenter() {}

private:
    SakuraGameScreenPresenter();

    SakuraGameScreenView& view;
};

#endif // SAKURAGAMESCREENPRESENTER_HPP
