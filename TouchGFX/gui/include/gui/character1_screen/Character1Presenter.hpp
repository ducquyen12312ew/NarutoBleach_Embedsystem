#ifndef CHARACTER1PRESENTER_HPP
#define CHARACTER1PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Character1View;

class Character1Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Character1Presenter(Character1View& v);

    virtual void activate();

    virtual void deactivate();

    virtual ~Character1Presenter() {}

private:
    Character1Presenter();

    Character1View& view;
};

#endif
