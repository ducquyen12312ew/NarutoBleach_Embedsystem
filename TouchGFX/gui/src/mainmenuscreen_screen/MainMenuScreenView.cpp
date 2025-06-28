#include <gui/mainmenuscreen_screen/MainMenuScreenView.hpp>

MainMenuScreenView::MainMenuScreenView() : audioInitialized(false)
{
}

void MainMenuScreenView::setupScreen()
{
    MainMenuScreenViewBase::setupScreen();

    if (!audioInitialized) {
        Audio_Init();
        audioInitialized = true;
    }

    Audio_PlayBackgroundMusic();
}

void MainMenuScreenView::tearDownScreen()
{
    Audio_StopBackgroundMusic();

    MainMenuScreenViewBase::tearDownScreen();
}

void MainMenuScreenView::flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (&src == &SELECT)
    {
        Audio_StopBackgroundMusic();

        application().gotoCharacterScreenNoTransition();
    }
    if (&src == &PLAY)
    {
        Audio_StopBackgroundMusic();

        application().gotoGameScreenScreenNoTransition();
    }
}
