#ifndef CHARACTER_2PRESENTER_HPP
#define CHARACTER_2PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Character_2View;

class Character_2Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Character_2Presenter(Character_2View& v);


    virtual void activate();

    virtual void deactivate();

    virtual ~Character_2Presenter() {}

private:
    Character_2Presenter();

    Character_2View& view;
};

#endif
