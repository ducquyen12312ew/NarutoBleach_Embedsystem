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

    virtual ~Character_2Presenter() {}

private:
    Character_2Presenter();

    Character_2View& view;
};

#endif // CHARACTER_2PRESENTER_HPP
