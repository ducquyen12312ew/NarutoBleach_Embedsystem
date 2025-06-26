#ifndef BACKUPSCREENVIEW_HPP
#define BACKUPSCREENVIEW_HPP

#include <gui_generated/backupscreen_screen/BackUpScreenViewBase.hpp>
#include <gui/backupscreen_screen/BackUpScreenPresenter.hpp>

class BackUpScreenView : public BackUpScreenViewBase
{
public:
    BackUpScreenView();
    virtual ~BackUpScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // BACKUPSCREENVIEW_HPP
