#ifndef SPONGEPRESENTER_HPP
#define SPONGEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SpongeView;

class SpongePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SpongePresenter(SpongeView& v);
    virtual void activate();
    virtual void deactivate();
    virtual ~SpongePresenter() {}

private:
    SpongePresenter();
    SpongeView& view;
};

#endif
