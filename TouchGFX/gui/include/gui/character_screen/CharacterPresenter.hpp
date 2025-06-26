#ifndef CHARACTERPRESENTER_HPP
#define CHARACTERPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class CharacterView;

class CharacterPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    CharacterPresenter(CharacterView& v);

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

    virtual ~CharacterPresenter() {}

private:
    CharacterPresenter();

    CharacterView& view;
};

#endif // CHARACTERPRESENTER_HPP
