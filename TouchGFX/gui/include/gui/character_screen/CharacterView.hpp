#ifndef CHARACTERVIEW_HPP
#define CHARACTERVIEW_HPP

#include <gui_generated/character_screen/CharacterViewBase.hpp>
#include <gui/character_screen/CharacterPresenter.hpp>
#include "main.h"

class CharacterView : public CharacterViewBase
{
public:
    CharacterView();
    virtual ~CharacterView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleClickEvent(const touchgfx::ClickEvent& evt);

protected:
    enum Character {
        NARUTO = 0,
        SAKURA = 1,
        SPONGE = 2,
        CHARACTER_COUNT = 3
    };

    int currentCharacterIndex;

    // Static variables to track unlock status
    static bool sakuraUnlocked;
    static bool spongeUnlocked;

    void showCharacter(Character character);
    void hideAllCharacters();
    void updateCharacterDisplay();
    bool isCharacterUnlocked(Character character);
    void testLED();
    void leftArrowClicked();
    void rightArrowClicked();

public:
    // Static methods to unlock characters (call from GameScreen)
    static void unlockSakura() { sakuraUnlocked = true; }
    static void unlockSponge() { spongeUnlocked = true; }
    static void resetProgress() { sakuraUnlocked = false; spongeUnlocked = false; }

    // Test method để unlock trong character screen
    void testUnlock();
};

#endif // CHARACTERVIEW_HPP
