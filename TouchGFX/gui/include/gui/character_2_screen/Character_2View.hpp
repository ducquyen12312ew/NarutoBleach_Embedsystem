#ifndef CHARACTER_2VIEW_HPP
#define CHARACTER_2VIEW_HPP

#include <gui_generated/character_2_screen/Character_2ViewBase.hpp>
#include <gui/character_2_screen/Character_2Presenter.hpp>
#include "main.h"

class Character_2View : public Character_2ViewBase
{
public:
    Character_2View();
    virtual ~Character_2View() {}
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
    void showCurrentCharacter();
    void hideAllCharacters();
    void hideAllButtons(); // Thêm hàm ẩn buttons
    void testLED();
    void leftArrowClicked();
    void rightArrowClicked();
};

#endif // CHARACTER_2VIEW_HPP
