#ifndef SAKURAFINALSCREENPRESENTER_HPP
#define SAKURAFINALSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SakuraFinalScreenView;

class SakuraFinalScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SakuraFinalScreenPresenter(SakuraFinalScreenView& v);
    virtual void activate();
    virtual void deactivate();
    virtual ~SakuraFinalScreenPresenter() {}

private:
    SakuraFinalScreenPresenter();
    SakuraFinalScreenView& view;
};

#endif
