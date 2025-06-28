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

    virtual void activate();

    virtual void deactivate();

    virtual ~Character_FinalPresenter() {}

private:
    Character_FinalPresenter();

    Character_FinalView& view;
};

#endif
