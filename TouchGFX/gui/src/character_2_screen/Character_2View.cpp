#include <gui/character_2_screen/Character_2View.hpp>

Character_2View::Character_2View() : currentCharacterIndex(NARUTO)
{
}

void Character_2View::setupScreen()
{
    Character_2ViewBase::setupScreen();

    currentCharacterIndex = NARUTO;

    hideAllCharacters();
    hideAllButtons();

    naruto.setVisible(true);
    narutobutton.setVisible(true);
    narutobutton.setTouchable(true);

    invalidate();
    testLED();
}

void Character_2View::tearDownScreen()
{
    Character_2ViewBase::tearDownScreen();
}

void Character_2View::handleClickEvent(const touchgfx::ClickEvent& evt)
{
    testLED();

    int16_t clickX = evt.getX();
    int16_t clickY = evt.getY();

    if (clickX >= 197 && clickX <= 239 && clickY >= 135 && clickY <= 189)
    {
        rightArrowClicked();
        return;
    }

    if (clickX >= 0 && clickX <= 42 && clickY >= 135 && clickY <= 189)
    {
        leftArrowClicked();
        return;
    }

    Character_2ViewBase::handleClickEvent(evt);
}

void Character_2View::leftArrowClicked()
{
    testLED();

    currentCharacterIndex--;
    if (currentCharacterIndex < 0) {
        currentCharacterIndex = CHARACTER_COUNT - 1;
    }

    showCurrentCharacter();
}

void Character_2View::rightArrowClicked()
{
    testLED();

    currentCharacterIndex++;
    if (currentCharacterIndex >= CHARACTER_COUNT) {
        currentCharacterIndex = 0;
    }

    showCurrentCharacter();
}

void Character_2View::showCurrentCharacter()
{
    hideAllCharacters();
    hideAllButtons();

    switch (currentCharacterIndex) {
    case NARUTO:
        naruto.setVisible(true);
        narutobutton.setVisible(true);
        narutobutton.setTouchable(true);
        break;

    case SAKURA:
        sakura.setVisible(true);
        sakurabutton.setVisible(true);
        sakurabutton.setTouchable(true);
        break;

    case SPONGE:
        sponge.setVisible(true);
        break;
    }

    invalidate();
}

void Character_2View::hideAllCharacters()
{
    naruto.setVisible(false);
    sakura.setVisible(false);
    sponge.setVisible(false);

    naruto.invalidate();
    sakura.invalidate();
    sponge.invalidate();
}

void Character_2View::hideAllButtons()
{
    sakurabutton.setVisible(false);
    sakurabutton.setTouchable(false);

    narutobutton.setVisible(false);
    narutobutton.setTouchable(false);

    sakurabutton.invalidate();
    narutobutton.invalidate();
}

void Character_2View::testLED()
{
    HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
}
