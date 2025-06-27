#include <gui/mainmenuscreen_screen/MainMenuScreenView.hpp>

MainMenuScreenView::MainMenuScreenView() : audioInitialized(false)
{
}

void MainMenuScreenView::setupScreen()
{
    MainMenuScreenViewBase::setupScreen();

    // Initialize audio system if not already done
    if (!audioInitialized) {
        Audio_Init();
        audioInitialized = true;
    }

    // Start playing background music when entering main menu
    Audio_PlayBackgroundMusic();
}

void MainMenuScreenView::tearDownScreen()
{
    // Stop background music when leaving main menu
    Audio_StopBackgroundMusic();

    MainMenuScreenViewBase::tearDownScreen();
}

void MainMenuScreenView::flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (&src == &SELECT)
    {
        // Stop background music before transitioning
        Audio_StopBackgroundMusic();

        //Select
        //When SELECT clicked change screen to Character
        //Go to Character with no screen transition
        application().gotoCharacterScreenNoTransition();
    }
    if (&src == &PLAY)
    {
        // Stop background music before transitioning
        Audio_StopBackgroundMusic();

        //GameScreen
        //When PLAY clicked change screen to GameScreen
        //Go to GameScreen with no screen transition
        application().gotoGameScreenScreenNoTransition();
    }
}
