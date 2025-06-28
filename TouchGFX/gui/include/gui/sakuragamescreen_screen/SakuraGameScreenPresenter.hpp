#ifndef SAKURAGAMESCREENPRESENTER_HPP
#define SAKURAGAMESCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SakuraGameScreenView;

class SakuraGameScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SakuraGameScreenPresenter(SakuraGameScreenView& v);
    virtual void activate();
    virtual void deactivate();
    virtual ~SakuraGameScreenPresenter() {}

private:
    SakuraGameScreenPresenter();
    SakuraGameScreenView& view;
};

#endif
