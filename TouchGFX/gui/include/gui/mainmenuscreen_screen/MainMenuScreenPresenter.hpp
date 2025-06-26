#ifndef MAINMENUSCREENPRESENTER_HPP
#define MAINMENUSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MainMenuScreenView;

class MainMenuScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    MainMenuScreenPresenter(MainMenuScreenView& v);

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

    virtual ~MainMenuScreenPresenter() {}

private:
    MainMenuScreenPresenter();

    MainMenuScreenView& view;
};

#endif // MAINMENUSCREENPRESENTER_HPP
