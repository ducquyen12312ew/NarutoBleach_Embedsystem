#include <gui/character_screen/CharacterView.hpp>

// Initialize static variables
bool CharacterView::sakuraUnlocked = false;
bool CharacterView::spongeUnlocked = false;

CharacterView::CharacterView() : currentCharacterIndex(NARUTO)
{

}

void CharacterView::setupScreen()
{
    CharacterViewBase::setupScreen();

    // Ban đầu chỉ hiện Naruto, ẩn tất cả khác
    hideAllCharacters();
    naruto.setVisible(true);

    invalidate();
    testLED();
}

void CharacterView::tearDownScreen()
{
    CharacterViewBase::tearDownScreen();
}

void CharacterView::handleClickEvent(const touchgfx::ClickEvent& evt)
{
    testLED();

    int16_t clickX = evt.getX();
    int16_t clickY = evt.getY();

    // Test unlock bằng cách touch vào giữa màn hình
    if (clickX >= 80 && clickX <= 160 && clickY >= 250 && clickY <= 300) {
        testUnlock();
        return;
    }

    // Check right arrow click
    if (clickX >= 197 && clickX <= 239 && clickY >= 135 && clickY <= 189)
    {
        rightArrowClicked();
        return;
    }

    // Check left arrow click
    if (clickX >= 0 && clickX <= 42 && clickY >= 135 && clickY <= 189)
    {
        leftArrowClicked();
        return;
    }

    CharacterViewBase::handleClickEvent(evt);
}

void CharacterView::leftArrowClicked()
{
    testLED();

    // Cycle through all characters (unlocked và locked)
    currentCharacterIndex--;
    if (currentCharacterIndex < 0) {
        currentCharacterIndex = CHARACTER_COUNT - 1;
    }

    // Show appropriate character or lock image
    hideAllCharacters();

    switch (currentCharacterIndex) {
        case NARUTO:
            naruto.setVisible(true);
            break;
        case SAKURA:
            if (sakuraUnlocked) {
                sakura.setVisible(true);
            } else {
                sakura_lock.setVisible(true);
            }
            break;
        case SPONGE:
            if (spongeUnlocked) {
                sponge.setVisible(true);
            } else {
                sponge_lock.setVisible(true);
            }
            break;
    }

    invalidate();
}

void CharacterView::rightArrowClicked()
{
    testLED();

    // Cycle through all characters (unlocked và locked)
    currentCharacterIndex++;
    if (currentCharacterIndex >= CHARACTER_COUNT) {
        currentCharacterIndex = 0;
    }

    // Show appropriate character or lock image
    hideAllCharacters();

    switch (currentCharacterIndex) {
        case NARUTO:
            naruto.setVisible(true);
            break;
        case SAKURA:
            if (sakuraUnlocked) {
                sakura.setVisible(true);
            } else {
                sakura_lock.setVisible(true);
            }
            break;
        case SPONGE:
            if (spongeUnlocked) {
                sponge.setVisible(true);
            } else {
                sponge_lock.setVisible(true);
            }
            break;
    }

    invalidate();
}

bool CharacterView::isCharacterUnlocked(Character character)
{
    switch (character) {
        case NARUTO:
            return true;  // Always unlocked
        case SAKURA:
            return sakuraUnlocked;  // Unlocked after map 1
        case SPONGE:
            return spongeUnlocked;  // Unlocked later
        case CHARACTER_COUNT:
            return false;
    }
    return false;
}

void CharacterView::updateCharacterDisplay()
{
    // Hide all lock images first
    sakura_lock.setVisible(false);
    sponge_lock.setVisible(false);

    // Show lock images for locked characters
    if (!sakuraUnlocked) {
        sakura_lock.setVisible(true);
    }

    if (!spongeUnlocked) {
        sponge_lock.setVisible(true);
    }

    invalidate();
}

void CharacterView::showCharacter(Character character)
{
    hideAllCharacters();

    // Update lock display first
    updateCharacterDisplay();

    // Show the selected character (normal or locked version)
    switch (character) {
        case NARUTO:
            naruto.setVisible(true);
            break;
        case SAKURA:
            if (sakuraUnlocked) {
                sakura.setVisible(true);
                sakura_lock.setVisible(false);
            } else {
                sakura_lock.setVisible(true);
                sakura.setVisible(false);
            }
            break;
        case SPONGE:
            if (spongeUnlocked) {
                sponge.setVisible(true);
                sponge_lock.setVisible(false);
            } else {
                sponge_lock.setVisible(true);
                sponge.setVisible(false);
            }
            break;
        case CHARACTER_COUNT:
            break;
    }

    invalidate();
}

void CharacterView::hideAllCharacters()
{
    naruto.setVisible(false);
    sponge.setVisible(false);
    sakura.setVisible(false);
    sakura_lock.setVisible(false);
    sponge_lock.setVisible(false);
}

void CharacterView::testLED()
{
    HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
}

void CharacterView::testUnlock()
{
    // Test function: Touch giữa màn hình để unlock characters
    static int unlockStep = 0;

    switch (unlockStep) {
        case 0:
            // First touch: unlock Sakura
            unlockSakura();
            unlockStep = 1;
            break;
        case 1:
            // Second touch: unlock Sponge
            unlockSponge();
            unlockStep = 2;
            break;
        case 2:
            // Third touch: reset all
            resetProgress();
            unlockStep = 0;
            currentCharacterIndex = NARUTO;
            break;
    }

    // Update display after unlock change
    hideAllCharacters();

    // Show current character if unlocked
    if (currentCharacterIndex == NARUTO) {
        naruto.setVisible(true);
    } else if (currentCharacterIndex == SAKURA && sakuraUnlocked) {
        sakura.setVisible(true);
    } else if (currentCharacterIndex == SPONGE && spongeUnlocked) {
        sponge.setVisible(true);
    } else {
        // If current character is locked, go back to Naruto
        currentCharacterIndex = NARUTO;
        naruto.setVisible(true);
    }

    // Always show lock images for locked characters
    if (!sakuraUnlocked) sakura_lock.setVisible(true);
    if (!spongeUnlocked) sponge_lock.setVisible(true);

    invalidate();
}
