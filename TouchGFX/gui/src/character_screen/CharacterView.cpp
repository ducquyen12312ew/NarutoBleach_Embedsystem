#include <gui/character_screen/CharacterView.hpp>

bool CharacterView::sakuraUnlocked = false;
bool CharacterView::spongeUnlocked = false;

CharacterView::CharacterView() : currentCharacterIndex(NARUTO)
{
}

void CharacterView::setupScreen()
{
    CharacterViewBase::setupScreen();
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

    if (clickX >= 80 && clickX <= 160 && clickY >= 250 && clickY <= 300) {
        testUnlock();
        return;
    }

    if (clickX >= 197 && clickX <= 239 && clickY >= 135 && clickY <= 189) {
        rightArrowClicked();
        return;
    }

    if (clickX >= 0 && clickX <= 42 && clickY >= 135 && clickY <= 189) {
        leftArrowClicked();
        return;
    }

    CharacterViewBase::handleClickEvent(evt);
}

void CharacterView::leftArrowClicked()
{
    testLED();
    currentCharacterIndex--;
    if (currentCharacterIndex < 0) {
        currentCharacterIndex = CHARACTER_COUNT - 1;
    }
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
    currentCharacterIndex++;
    if (currentCharacterIndex >= CHARACTER_COUNT) {
        currentCharacterIndex = 0;
    }
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
            return true;
        case SAKURA:
            return sakuraUnlocked;
        case SPONGE:
            return spongeUnlocked;
        case CHARACTER_COUNT:
            return false;
    }
    return false;
}

void CharacterView::updateCharacterDisplay()
{
    sakura_lock.setVisible(false);
    sponge_lock.setVisible(false);

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
    updateCharacterDisplay();

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
    static int unlockStep = 0;

    switch (unlockStep) {
        case 0:
            unlockSakura();
            unlockStep = 1;
            break;
        case 1:
            unlockSponge();
            unlockStep = 2;
            break;
        case 2:
            resetProgress();
            unlockStep = 0;
            currentCharacterIndex = NARUTO;
            break;
    }

    hideAllCharacters();

    if (currentCharacterIndex == NARUTO) {
        naruto.setVisible(true);
    } else if (currentCharacterIndex == SAKURA && sakuraUnlocked) {
        sakura.setVisible(true);
    } else if (currentCharacterIndex == SPONGE && spongeUnlocked) {
        sponge.setVisible(true);
    } else {
        currentCharacterIndex = NARUTO;
        naruto.setVisible(true);
    }

    if (!sakuraUnlocked) sakura_lock.setVisible(true);
    if (!spongeUnlocked) sponge_lock.setVisible(true);

    invalidate();
}
