#ifndef CHARACTER_FINALPRESENTER_HPP
#define CHARACTER_FINALPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Character_FinalView;

class Character_FinalPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Character_FinalPresenter(Character_FinalView& v);

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

    virtual ~Character_FinalPresenter() {}

private:
    Character_FinalPresenter();

    Character_FinalView& view;
};

#endif // CHARACTER_FINALPRESENTER_HPP
