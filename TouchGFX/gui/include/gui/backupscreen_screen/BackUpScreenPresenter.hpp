#ifndef BACKUPSCREENPRESENTER_HPP
#define BACKUPSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class BackUpScreenView;

class BackUpScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    BackUpScreenPresenter(BackUpScreenView& v);
    virtual void activate();

    virtual void deactivate();

    virtual ~BackUpScreenPresenter() {}

private:
    BackUpScreenPresenter();

    BackUpScreenView& view;
};

#endif
