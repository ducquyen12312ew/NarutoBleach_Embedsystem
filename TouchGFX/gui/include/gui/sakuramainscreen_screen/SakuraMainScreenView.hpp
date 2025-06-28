#ifndef SAKURAMAINSCREENVIEW_HPP
#define SAKURAMAINSCREENVIEW_HPP

#include <gui_generated/sakuramainscreen_screen/SakuraMainScreenViewBase.hpp>
#include <gui/sakuramainscreen_screen/SakuraMainScreenPresenter.hpp>

class SakuraMainScreenView : public SakuraMainScreenViewBase
{
public:
    SakuraMainScreenView();
    virtual ~SakuraMainScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif
