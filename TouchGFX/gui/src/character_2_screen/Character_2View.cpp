#include <gui/character_2_screen/Character_2View.hpp>

Character_2View::Character_2View() : currentCharacterIndex(NARUTO)
{
}

void Character_2View::setupScreen()
{
    Character_2ViewBase::setupScreen();

    // Reset về Naruto làm default
    currentCharacterIndex = NARUTO;

    // ẨN TẤT CẢ TRƯỚC
    hideAllCharacters();
    hideAllButtons();

    // CHỈ HIỂN THỊ NARUTO BAN ĐẦU (có button cho Naruto)
    naruto.setVisible(true);
    narutobutton.setVisible(true);
    narutobutton.setTouchable(true);

    invalidate();
    testLED(); // Confirm setup
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

    // Check right arrow click (197, 135, 42x54)
    if (clickX >= 197 && clickX <= 239 && clickY >= 135 && clickY <= 189)
    {
        rightArrowClicked();
        return;
    }

    // Check left arrow click (0, 135, 42x54)
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

    // Cycle through characters
    currentCharacterIndex--;
    if (currentCharacterIndex < 0) {
        currentCharacterIndex = CHARACTER_COUNT - 1;
    }

    // Show character tương ứng
    showCurrentCharacter();
}

void Character_2View::rightArrowClicked()
{
    testLED();

    // Cycle through characters
    currentCharacterIndex++;
    if (currentCharacterIndex >= CHARACTER_COUNT) {
        currentCharacterIndex = 0;
    }

    // Show character tương ứng
    showCurrentCharacter();
}

void Character_2View::showCurrentCharacter()
{
    // ẨN TẤT CẢ TRƯỚC
    hideAllCharacters();
    hideAllButtons();

    // HIỂN THỊ CHARACTER VÀ BUTTON THEO INDEX
    switch (currentCharacterIndex) {
    case NARUTO:
        naruto.setVisible(true);
        // ===== HIỂN THỊ NARUTO BUTTON ĐỂ CHUYỂN SANG NARUTO GAME SCREEN =====
        narutobutton.setVisible(true);
        narutobutton.setTouchable(true);
        break;

    case SAKURA:
        sakura.setVisible(true);
        // ===== HIỂN THỊ SAKURA BUTTON ĐỂ CHUYỂN SANG SAKURA MAIN SCREEN =====
        sakurabutton.setVisible(true);
        sakurabutton.setTouchable(true);
        break;

    case SPONGE:
        sponge.setVisible(true);
        // Sponge không có button
        break;
    }

    invalidate();
}

void Character_2View::hideAllCharacters()
{
    // ẨN TẤT CẢ NHÂN VẬT
    naruto.setVisible(false);
    sakura.setVisible(false);
    sponge.setVisible(false);

    // INVALIDATE TẤT CẢ
    naruto.invalidate();
    sakura.invalidate();
    sponge.invalidate();
}

void Character_2View::hideAllButtons()
{
    // ẨN TẤT CẢ BUTTON
    sakurabutton.setVisible(false);
    sakurabutton.setTouchable(false);

    narutobutton.setVisible(false);
    narutobutton.setTouchable(false);

    // INVALIDATE BUTTONS
    sakurabutton.invalidate();
    narutobutton.invalidate();
}

void Character_2View::testLED()
{
    HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
}
