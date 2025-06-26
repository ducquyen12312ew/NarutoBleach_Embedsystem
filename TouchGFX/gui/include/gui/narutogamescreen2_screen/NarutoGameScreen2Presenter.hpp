#ifndef NARUTOGAMESCREEN2PRESENTER_HPP
#define NARUTOGAMESCREEN2PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class NarutoGameScreen2View;

class NarutoGameScreen2Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    NarutoGameScreen2Presenter(NarutoGameScreen2View& v);

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

    virtual ~NarutoGameScreen2Presenter() {}

private:
    NarutoGameScreen2Presenter();

    NarutoGameScreen2View& view;
};

#endif // NARUTOGAMESCREEN2PRESENTER_HPP
