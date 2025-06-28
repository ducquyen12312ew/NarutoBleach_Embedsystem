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

    virtual void activate();

    virtual void deactivate();

    virtual ~CharacterPresenter() {}

private:
    CharacterPresenter();

    CharacterView& view;
};

#endif
