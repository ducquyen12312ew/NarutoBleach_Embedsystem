#ifndef SAKURAFINALSCREENVIEW_HPP
#define SAKURAFINALSCREENVIEW_HPP

#include <gui_generated/sakurafinalscreen_screen/SakuraFinalScreenViewBase.hpp>
#include <gui/sakurafinalscreen_screen/SakuraFinalScreenPresenter.hpp>

class SakuraFinalScreenView : public SakuraFinalScreenViewBase
{
public:
    SakuraFinalScreenView();
    virtual ~SakuraFinalScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SAKURAFINALSCREENVIEW_HPP
