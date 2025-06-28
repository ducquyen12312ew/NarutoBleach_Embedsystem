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

    virtual void activate();

    virtual void deactivate();

    virtual ~NarutoGameScreen2Presenter() {}

private:
    NarutoGameScreen2Presenter();

    NarutoGameScreen2View& view;
};

#endif
