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

    virtual void activate();

    virtual void deactivate();

    virtual ~MainMenuScreenPresenter() {}

private:
    MainMenuScreenPresenter();

    MainMenuScreenView& view;
};

#endif
