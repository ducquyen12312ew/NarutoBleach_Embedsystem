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

    virtual ~SpongePresenter() {}

private:
    SpongePresenter();

    SpongeView& view;
};

#endif // SPONGEPRESENTER_HPP
