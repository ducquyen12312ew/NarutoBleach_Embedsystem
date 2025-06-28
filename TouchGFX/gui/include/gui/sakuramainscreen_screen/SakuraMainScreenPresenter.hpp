#ifndef SAKURAMAINSCREENPRESENTER_HPP
#define SAKURAMAINSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SakuraMainScreenView;

class SakuraMainScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SakuraMainScreenPresenter(SakuraMainScreenView& v);
    virtual void activate();
    virtual void deactivate();
    virtual ~SakuraMainScreenPresenter() {}

private:
    SakuraMainScreenPresenter();
    SakuraMainScreenView& view;
};

#endif
