// MainMenuScreenView.hpp
#ifndef MAINMENUSCREENVIEW_HPP
#define MAINMENUSCREENVIEW_HPP

#include <gui_generated/mainmenuscreen_screen/MainMenuScreenViewBase.hpp>
#include <gui/mainmenuscreen_screen/MainMenuScreenPresenter.hpp>

class MainMenuScreenView : public MainMenuScreenViewBase
{
public:
    MainMenuScreenView();
    virtual ~MainMenuScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

protected:

private:
};

#endif // MAINMENUSCREENVIEW_HPP
