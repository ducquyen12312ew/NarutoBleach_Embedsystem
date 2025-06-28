#ifndef MAINMENUSCREENVIEW_HPP
#define MAINMENUSCREENVIEW_HPP

#include <gui_generated/mainmenuscreen_screen/MainMenuScreenViewBase.hpp>
#include <gui/mainmenuscreen_screen/MainMenuScreenPresenter.hpp>


extern "C" {
    #include "audio_system.h"
}

class MainMenuScreenView : public MainMenuScreenViewBase
{
public:
    MainMenuScreenView();
    virtual ~MainMenuScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);

protected:
private:
    bool audioInitialized;
};

#endif
