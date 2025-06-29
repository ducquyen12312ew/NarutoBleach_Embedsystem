#include <gui/narutogamescreen2_screen/NarutoGameScreen2View.hpp>
#include <touchgfx/Color.hpp>
#include "stm32f4xx_hal.h"
#include <gui/character_screen/CharacterView.hpp>

NarutoGameScreen2View::NarutoGameScreen2View()
{
    resetTimer = 0;
    resetTimerActive = false;
    randomSeed = 12345;

    fireAnimationTimer = 0;
    fireAnimationActive = false;
    fireAnimationState = 0;
    fireLoopCounter = 0;
    specialFireToggle = false;

    bossPhase = 1;
    bossMaxHP = 130;
    bossCurrentHP = 130;
    rageMode = false;
    rageModeTimer = 0;

    easterEggActivated = false;
    easterEggTimer = 0;
}

void NarutoGameScreen2View::setupScreen()
{
    NarutoGameScreen2ViewBase::setupScreen();
    initGPIO();

    randomSeed = HAL_GetTick();
    if (randomSeed == 0) randomSeed = 12345;
    randomSeed = (randomSeed * 1664525 + 1013904223);

    playerHP = 100;
    bossCurrentHP = bossMaxHP;
    playerChakraLevel = 0;
    aiChakraLevel = 0;
    menuOpen = false;
    gameEnded = false;

    playerDefending = false;
    aiDefending = false;
    playerSpecialUsed = false;
    aiSpecialUsed = false;

    bossPhase = 1;
    rageMode = false;
    rageModeTimer = 0;

    turnCounter = 0;
    playerPowerUpType = 0;
    aiPowerUpType = 0;
    playerPowerUpActive = false;
    aiPowerUpActive = false;
    playerActionDone = false;
    aiActionDone = false;

    aiPersonality = 0;

    resetTimer = 0;
    resetTimerActive = false;
    autoReturnTimer = 0;
    autoReturnTimerActive = false;

    hideAllPlayerPowerUps();
    hideAllAIPowerUps();

    updatePlayerHPDisplay();
    updateBossHPDisplay();
    updatePlayerChakraDisplay();
    updateAIChakraDisplay();

    resetNarutoState();
    resetBleachState();

    fireAnimationTimer = 0;
    fireAnimationActive = false;
    fireAnimationState = 0;
    fireLoopCounter = 0;
    specialFireToggle = false;

    hideAllFireEffects();
    forceBossHPVisible();

    easterEggActivated = false;
    easterEggTimer = 0;
}

void NarutoGameScreen2View::hideAllFireEffects()
{
    fire1.setVisible(false);
    fire2.setVisible(false);
    bleach_spe_fire.setVisible(false);

    fire1.invalidate();
    fire2.invalidate();
    bleach_spe_fire.invalidate();
}

void NarutoGameScreen2View::startAttackFireAnimation()
{
    hideAllFireEffects();
    fireAnimationTimer = 0;
    fireAnimationActive = true;
    fireAnimationState = 1;

    fire1.setVisible(true);
    fire1.invalidate();

    toggleLED();
}

void NarutoGameScreen2View::startSpecialFireAnimation()
{
    hideAllFireEffects();
    fireAnimationTimer = 0;
    fireAnimationActive = true;
    fireAnimationState = 3;
    fireLoopCounter = 0;
    specialFireToggle = true;

    bleach_spe_fire.setVisible(true);
    bleach_spe_fire.invalidate();

    for (int i = 0; i < 4; i++) {
        toggleLED();
    }
}

void NarutoGameScreen2View::updateFireAnimation()
{
    if (!fireAnimationActive) return;

    fireAnimationTimer += 16;

    switch (fireAnimationState) {
        case 1:
            if (fireAnimationTimer >= FIRE_ANIMATION_DURATION) {
                fire1.setVisible(false);
                fire2.setVisible(true);
                fire1.invalidate();
                fire2.invalidate();

                fireAnimationState = 2;
                fireAnimationTimer = 0;
                toggleLED();
            }
            break;

        case 2:
            if (fireAnimationTimer >= FIRE_ANIMATION_DURATION) {
                hideAllFireEffects();
                fireAnimationActive = false;
                fireAnimationState = 0;
                fireAnimationTimer = 0;

                if (!gameEnded && bossCurrentHP > 0) {
                    aiActionDone = true;
                    toggleLED();
                }
            }
            break;

        case 3:
            if (fireAnimationTimer >= FIRE_LOOP_INTERVAL) {
                specialFireToggle = !specialFireToggle;
                bleach_spe_fire.setVisible(specialFireToggle);
                bleach_spe_fire.invalidate();

                fireAnimationTimer = 0;
                fireLoopCounter += FIRE_LOOP_INTERVAL;

                toggleLED();
            }

            if (fireLoopCounter >= 3000) {
                hideAllFireEffects();

                bleach_spe.setVisible(false);
                bleach_spe.invalidate();

                fireAnimationActive = false;
                fireAnimationState = 0;
                fireAnimationTimer = 0;
                fireLoopCounter = 0;

                if (!gameEnded && bossCurrentHP > 0) {
                    aiActionDone = true;
                    aiSpecialUsed = true;

                    for (int i = 0; i < 5; i++) {
                        toggleLED();
                    }
                }
            }
            break;
    }
}

void NarutoGameScreen2View::tearDownScreen()
{
    NarutoGameScreen2ViewBase::tearDownScreen();
}

void NarutoGameScreen2View::initGPIO()
{
    __HAL_RCC_GPIOG_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
}

void NarutoGameScreen2View::toggleLED()
{
    HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
}

void NarutoGameScreen2View::hideActionButtons()
{
    btn_attack.setVisible(false);
    btn_def.setVisible(false);
    btn_spe.setVisible(false);
    btn_arrow1.setVisible(true);
    btn_arrow2.setVisible(false);

    btn_attack.invalidate();
    btn_def.invalidate();
    btn_spe.invalidate();
    btn_arrow1.invalidate();
    btn_arrow2.invalidate();

    menuOpen = false;
}

void NarutoGameScreen2View::showActionButtons()
{
    btn_attack.setVisible(true);
    btn_def.setVisible(true);
    btn_spe.setVisible(true);
    btn_arrow1.setVisible(false);
    btn_arrow2.setVisible(true);

    btn_attack.invalidate();
    btn_def.invalidate();
    btn_spe.invalidate();
    btn_arrow1.invalidate();
    btn_arrow2.invalidate();

    menuOpen = true;
}

void NarutoGameScreen2View::toggleActionMenu()
{
    if (menuOpen) {
        hideActionButtons();
    } else {
        showActionButtons();
    }
    toggleLED();
}

void NarutoGameScreen2View::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED) {
        if (!gameEnded && !easterEggActivated) {
            if (evt.getX() >= 94 && evt.getX() <= 240 &&
                evt.getY() >= 62 && evt.getY() <= 234) {

                activateEasterEgg();
                return;
            }
        }

        if (evt.getX() < 50 && evt.getY() < 50) {
            bossCurrentHP -= 30;
            if (bossCurrentHP < 0) bossCurrentHP = 0;
            updateBossHPDisplay();
            return;
        }

        if (evt.getX() > 190 && evt.getY() < 50) {
            playerHP = 0;
            updatePlayerHPDisplay();
            return;
        }

        if (evt.getX() < 50 && evt.getY() > 270) {
            for (int i = 0; i < 3; i++) {
                toggleLED();
            }
            return;
        }

        if (evt.getX() > 190 && evt.getY() > 270 && !gameEnded) {
            givePlayerPowerUp();
            return;
        }

        if (evt.getX() > 100 && evt.getX() < 140 && evt.getY() > 270 && !gameEnded) {
            for (int i = 0; i < turnCounter + 1; i++) {
                toggleLED();
            }

            playerActionDone = true;
            aiActionDone = true;
            checkTurnComplete();
            return;
        }

        if (gameEnded) {
            if (btn_attack.getAbsoluteRect().intersect(evt.getX(), evt.getY()) ||
                btn_def.getAbsoluteRect().intersect(evt.getX(), evt.getY()) ||
                btn_spe.getAbsoluteRect().intersect(evt.getX(), evt.getY()) ||
                btn_arrow1.getAbsoluteRect().intersect(evt.getX(), evt.getY()) ||
                btn_arrow2.getAbsoluteRect().intersect(evt.getX(), evt.getY())) {
                return;
            }
        }

        if (!gameEnded) {
            if (btn_arrow1.getAbsoluteRect().intersect(evt.getX(), evt.getY())) {
                toggleActionMenu();
                return;
            }

            if (btn_arrow2.getAbsoluteRect().intersect(evt.getX(), evt.getY())) {
                toggleActionMenu();
                return;
            }

            if (menuOpen) {
                if (btn_attack.getAbsoluteRect().intersect(evt.getX(), evt.getY())) {
                    playerAttack();
                    if (!gameEnded) performAIAction();
                    return;
                }
                else if (btn_def.getAbsoluteRect().intersect(evt.getX(), evt.getY())) {
                    playerDefend();
                    if (!gameEnded) performAIAction();
                    return;
                }
                else if (btn_spe.getAbsoluteRect().intersect(evt.getX(), evt.getY())) {
                    if (playerChakraLevel >= 100) {
                        playerSpecial();
                        if (!gameEnded) performAIAction();
                    }
                    return;
                }
            }
        }
    }

    NarutoGameScreen2ViewBase::handleClickEvent(evt);
}

void NarutoGameScreen2View::handleDragEvent(const DragEvent& evt)
{
    NarutoGameScreen2ViewBase::handleDragEvent(evt);
}

void NarutoGameScreen2View::handleTickEvent()
{
    static int frameCounter = 0;
    if (frameCounter < 10 && !gameEnded) {
        frameCounter++;
        updateBossHPDisplay();

        if (frameCounter == 5) {
            toggleLED();
            toggleLED();
            toggleLED();
        }
    }

    if (easterEggActivated) {
        easterEggTimer += 16;

        if (easterEggTimer >= EASTER_EGG_WIN_TIME) {
            HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);

            application().gotoCharacter1ScreenNoTransition();
            return;
        }
        return;
    }

    if (!gameEnded) {
        if (resetTimerActive) {
            resetTimer += 16;
            if (resetTimer >= RESET_TIME_MS) {
                resetNarutoState();
                resetBleachState();
                resetTimerActive = false;
                resetTimer = 0;
                playerDefending = false;
                aiDefending = false;

                if (!fireAnimationActive) {
                    checkTurnComplete();
                }
            }
        }

        if (rageMode) {
            rageModeTimer += 16;
        }

        updateFireAnimation();

    } else {
        if (autoReturnTimerActive) {
            autoReturnTimer += 16;

            if (autoReturnTimer >= AUTO_RETURN_TIME_MS) {
                HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
                application().gotoBackUpScreenScreenNoTransition();
                return;
            }
        }
    }

    NarutoGameScreen2ViewBase::handleTickEvent();
}

void NarutoGameScreen2View::hideAllBossHPWidgets()
{
    BITMAP_HP_130_ID_1.setVisible(false);
    BITMAP_HP_120_ID_1.setVisible(false);
    BITMAP_HP_110_ID_1.setVisible(false);
    BITMAP_HP_100_ID_1.setVisible(false);
    BITMAP_HP_90_ID_1.setVisible(false);
    BITMAP_HP_80_ID_1.setVisible(false);
    BITMAP_HP_70_ID_1.setVisible(false);
    BITMAP_HP_60_ID_1.setVisible(false);
    BITMAP_HP_50_ID_1.setVisible(false);
    BITMAP_HP_40_ID_1.setVisible(false);
    BITMAP_HP_30_ID_1.setVisible(false);
    BITMAP_HP_20_ID_1.setVisible(false);
    BITMAP_HP_10_ID_1.setVisible(false);
    BITMAP_HP_0_ID_1.setVisible(false);

    BITMAP_HP_130_ID_1.invalidate();
    BITMAP_HP_120_ID_1.invalidate();
    BITMAP_HP_110_ID_1.invalidate();
    BITMAP_HP_100_ID_1.invalidate();
    BITMAP_HP_90_ID_1.invalidate();
    BITMAP_HP_80_ID_1.invalidate();
    BITMAP_HP_70_ID_1.invalidate();
    BITMAP_HP_60_ID_1.invalidate();
    BITMAP_HP_50_ID_1.invalidate();
    BITMAP_HP_40_ID_1.invalidate();
    BITMAP_HP_30_ID_1.invalidate();
    BITMAP_HP_20_ID_1.invalidate();
    BITMAP_HP_10_ID_1.invalidate();
    BITMAP_HP_0_ID_1.invalidate();
}

void NarutoGameScreen2View::forceBossHPVisible()
{
    if (bossCurrentHP >= 125) {
        BITMAP_HP_130_ID_1.setVisible(true);
        BITMAP_HP_130_ID_1.setAlpha(255);
        BITMAP_HP_130_ID_1.invalidate();

        toggleLED();
        toggleLED();
        toggleLED();
        toggleLED();
        toggleLED();
    }
}

void NarutoGameScreen2View::checkTurnComplete()
{
    if (playerActionDone && aiActionDone) {
        turnCounter++;

        toggleLED();

        if (!playerSpecialUsed && !aiSpecialUsed) {
            int powerUpChance = generateRandomNumber(1, 100);
            if (powerUpChance <= 25) {
                givePlayerPowerUp();
            }
        }

        if (playerPowerUpActive && (playerPowerUpType == 2 || playerPowerUpType == 3 || playerPowerUpType == 4)) {
            clearPowerUpAfterUse(true);
        }

        playerActionDone = false;
        aiActionDone = false;
        playerSpecialUsed = false;
        aiSpecialUsed = false;
    }
}

void NarutoGameScreen2View::givePlayerPowerUp()
{
    for (int i = 0; i < 3; i++) {
        toggleLED();
    }

    int playerRoll = generateRandomNumber(1, 100);
    int newPlayerPowerUp;

    if (playerRoll >= 1 && playerRoll <= 30) {
        newPlayerPowerUp = 4;
    } else if (playerRoll >= 31 && playerRoll <= 55) {
        newPlayerPowerUp = 1;
    } else if (playerRoll >= 56 && playerRoll <= 80) {
        newPlayerPowerUp = 2;
    } else {
        newPlayerPowerUp = 3;
    }

    playerPowerUpType = newPlayerPowerUp;
    playerPowerUpActive = true;
    applyInstantPowerUp(true, playerPowerUpType);
    updatePlayerPowerUpDisplay();
}

void NarutoGameScreen2View::applyInstantPowerUp(bool isPlayer, int type)
{
    if (!isPlayer) return;

    switch (type) {
        case 1:
            break;

        case 2:
            playerHP += 8;
            if (playerHP > 100) playerHP = 100;
            updatePlayerHPDisplay();
            break;

        case 3:
            playerChakraLevel += 20;
            if (playerChakraLevel > 100) playerChakraLevel = 100;
            updatePlayerChakraDisplay();
            break;

        case 4:
            break;
    }
}

void NarutoGameScreen2View::updatePlayerPowerUpDisplay()
{
    hideAllPlayerPowerUps();

    if (!playerPowerUpActive) return;

    switch (playerPowerUpType) {
        case 1:
            x2dame_1.setVisible(true);
            x2dame_1.invalidate();
            break;

        case 2:
            healthplus_1.setVisible(true);
            healthplus_1.setAlpha(255);
            healthplus_1.setPosition(9, 61, 32, 37);
            healthplus_1.invalidate();
            healthplus_1.setVisible(false);
            healthplus_1.setVisible(true);
            healthplus_1.invalidate();
            break;

        case 3:
            chakragain_1.setVisible(true);
            chakragain_1.setAlpha(255);
            chakragain_1.setPosition(9, 61, 32, 37);
            chakragain_1.invalidate();
            chakragain_1.setVisible(false);
            chakragain_1.setVisible(true);
            chakragain_1.invalidate();
            break;

        case 4:
            sheild_1.setVisible(true);
            sheild_1.setAlpha(255);
            sheild_1.setPosition(8, 59, 32, 37);
            sheild_1.invalidate();
            break;
    }

    invalidate();
}

void NarutoGameScreen2View::updateAIPowerUpDisplay()
{
}

void NarutoGameScreen2View::hideAllPlayerPowerUps()
{
    x2dame_1.setVisible(false);
    healthplus_1.setVisible(false);
    chakragain_1.setVisible(false);
    sheild_1.setVisible(false);

    x2dame_1.invalidate();
    healthplus_1.invalidate();
    chakragain_1.invalidate();
    sheild_1.invalidate();
}

void NarutoGameScreen2View::hideAllAIPowerUps()
{
    x2dame_2.setVisible(false);
    healthplus_2.setVisible(false);
    chakragain_2.setVisible(false);
    sheild_2.setVisible(false);

    x2dame_2.invalidate();
    healthplus_2.invalidate();
    chakragain_2.invalidate();
    sheild_2.invalidate();
}

void NarutoGameScreen2View::clearPowerUpAfterUse(bool isPlayer)
{
    if (isPlayer) {
        playerPowerUpActive = false;
        playerPowerUpType = 0;
        updatePlayerPowerUpDisplay();
    } else {
        aiPowerUpActive = false;
        aiPowerUpType = 0;
        updateAIPowerUpDisplay();
    }
}

int NarutoGameScreen2View::generateRandomNumber(int min, int max)
{
    randomSeed = (randomSeed * 1664525 + 1013904223) & 0x7fffffff;

    uint32_t mixed = randomSeed;
    mixed ^= mixed >> 16;
    mixed *= 0x85ebca6b;
    mixed ^= mixed >> 13;
    mixed *= 0xc2b2ae35;
    mixed ^= mixed >> 16;

    return min + (mixed % (max - min + 1));
}

int NarutoGameScreen2View::calculateDamage(int baseDamage, bool isDefending, bool hasDoubleAttack, bool hasShield)
{
    if (hasShield) {
        return 0;
    }

    int finalDamage = baseDamage;

    if (hasDoubleAttack) {
        finalDamage = (int)(baseDamage * 2.5);
    }

    if (isDefending) {
        finalDamage = finalDamage / 2;

        if (finalDamage < 1) finalDamage = 1;
    }

    return finalDamage;
}

void NarutoGameScreen2View::playerAttack()
{
    resetNarutoState();
    naruto.setVisible(false);
    naruto_atk.setVisible(true);
    naruto_atk.invalidate();
    naruto.invalidate();

    int baseDamage = generateRandomNumber(3, 8);
    bool hasDoubleAttack = (playerPowerUpActive && playerPowerUpType == 1);
    int actualDamage = calculateDamage(baseDamage, aiDefending, hasDoubleAttack, false);

    bossCurrentHP -= actualDamage;
    if (bossCurrentHP < 0) bossCurrentHP = 0;

    playerChakraLevel += generateRandomNumber(5, 10);
    if (playerChakraLevel > 100) playerChakraLevel = 100;

    if (hasDoubleAttack) {
        clearPowerUpAfterUse(true);
    }

    updateBossHPDisplay();
    updatePlayerChakraDisplay();
    toggleLED();

    playerActionDone = true;
    playerSpecialUsed = false;

    if (bossCurrentHP <= 0) {
        startResetTimer();
        return;
    }
    startResetTimer();
}

void NarutoGameScreen2View::playerDefend()
{
    resetNarutoState();
    naruto.setVisible(false);
    naruto_def.setVisible(true);
    naruto_def.invalidate();
    naruto.invalidate();

    playerDefending = true;

    int healAmount = generateRandomNumber(3, 7);
    playerHP += healAmount;
    if (playerHP > 100) playerHP = 100;

    playerChakraLevel += generateRandomNumber(10, 18);
    if (playerChakraLevel > 100) playerChakraLevel = 100;

    if (playerPowerUpActive && playerPowerUpType == 1) {
        clearPowerUpAfterUse(true);
    }

    updatePlayerHPDisplay();
    updatePlayerChakraDisplay();
    toggleLED();

    playerActionDone = true;
    playerSpecialUsed = false;

    startResetTimer();
}

void NarutoGameScreen2View::playerSpecial()
{
    if (gameEnded || playerChakraLevel < 100) return;

    resetNarutoState();
    naruto.setVisible(false);
    naruto_spe.setVisible(true);
    naruto_spe.invalidate();
    naruto.invalidate();

    int baseDamage = 40;

    bool bossHasShield = false;

    int actualDamage = calculateSpecialDamage(baseDamage, aiDefending, bossHasShield);

    bossCurrentHP -= actualDamage;
    if (bossCurrentHP < 0) bossCurrentHP = 0;

    playerChakraLevel = 0;

    updateBossHPDisplay();
    updatePlayerChakraDisplay();

    for (int i = 0; i < 6; i++) {
        toggleLED();
    }

    playerActionDone = true;
    playerSpecialUsed = true;

    if (bossCurrentHP <= 0) {
        if (!gameEnded) {
            endGame(true);
        }
    }

    toggleLED();
    startResetTimer();
}

void NarutoGameScreen2View::performAIAction()
{
    if (gameEnded || bossCurrentHP <= 0) {
        return;
    }

    if (playerHP <= 0) {
        return;
    }

    int action = 1;

    if (aiPowerUpActive) {
        switch(aiPowerUpType) {
            case 1:
                if (aiChakraLevel >= 100) {
                    int roll = generateRandomNumber(1, 100);
                    if (roll <= 80) {
                        action = 1;
                    } else if (roll <= 90) {
                        action = 3;
                    } else {
                        action = 2;
                    }
                } else {
                    if (generateRandomNumber(1, 100) <= 90) {
                        action = 1;
                    } else {
                        action = 2;
                    }
                }
                break;

            case 4:
                if (aiChakraLevel >= 100) {
                    if (generateRandomNumber(1, 100) <= 50) {
                        action = 3;
                    } else {
                        action = 1;
                    }
                } else {
                    action = 1;
                }
                break;

            case 2:
            case 3:
            default:
                break;
        }
    }

    if (!aiPowerUpActive || (aiPowerUpType != 1 && aiPowerUpType != 4)) {
        int aiHPPercent = (bossCurrentHP * 100) / bossMaxHP;
        int playerHPPercent = (playerHP * 100) / 100;

        bool playerCanSpecial = (playerChakraLevel >= 100);

        int baseDefendBonus = 0;
        if (playerCanSpecial) {
            baseDefendBonus = 25;
        }

        switch(aiPersonality)
        {
            case 0:
                if (aiChakraLevel >= 100) {
                    int specialChance = 60;
                    int defendChance = 40 + baseDefendBonus;

                    if (aiHPPercent >= 80) {
                        defendChance = 50 + baseDefendBonus;
                        specialChance = 50 - (baseDefendBonus / 2);
                    } else if (aiHPPercent >= 60) {
                        defendChance = 40 + baseDefendBonus;
                        specialChance = 60 - (baseDefendBonus / 2);
                    } else if (aiHPPercent >= 30) {
                        defendChance = 30 + baseDefendBonus;
                        specialChance = 70 - (baseDefendBonus / 2);
                    } else {
                        defendChance = 20 + baseDefendBonus;
                        specialChance = 80 - (baseDefendBonus / 2);
                    }

                    if (playerHPPercent <= 25) {
                        specialChance += 15;
                        defendChance -= 10;
                    }

                    if (defendChance > 70) defendChance = 70;
                    if (specialChance < 30) specialChance = 30;

                    int roll = generateRandomNumber(1, 100);
                    if (roll <= defendChance) {
                        action = 2;
                    } else {
                        action = 3;
                    }
                } else {
                    if (aiHPPercent >= 70) {
                        int defendChance = 30 + baseDefendBonus;
                        if (defendChance > 70) defendChance = 70;
                        if (generateRandomNumber(1, 100) <= defendChance) {
                            action = 2;
                        } else {
                            action = 1;
                        }
                    } else if (aiHPPercent >= 40) {
                        int defendChance = 50 + baseDefendBonus;
                        if (defendChance > 80) defendChance = 80;
                        if (generateRandomNumber(1, 100) <= defendChance) {
                            action = 2;
                        } else {
                            action = 1;
                        }
                    } else {
                        int defendChance = 30 + baseDefendBonus;
                        if (defendChance > 60) defendChance = 60;
                        if (generateRandomNumber(1, 100) <= defendChance) {
                            action = 2;
                        } else {
                            action = 1;
                        }
                    }
                }
                break;

            case 1:
                if (aiChakraLevel >= 100) {
                    int defendChance = 60 + baseDefendBonus;
                    int specialChance = 40 - (baseDefendBonus / 2);

                    if (aiHPPercent >= 80) {
                        defendChance = 70 + baseDefendBonus;
                        specialChance = 30 - (baseDefendBonus / 2);
                    } else if (aiHPPercent >= 50) {
                        defendChance = 60 + baseDefendBonus;
                        specialChance = 40 - (baseDefendBonus / 2);
                    } else if (aiHPPercent >= 25) {
                        defendChance = 45 + baseDefendBonus;
                        specialChance = 55 - (baseDefendBonus / 2);
                    } else {
                        defendChance = 30 + baseDefendBonus;
                        specialChance = 70 - (baseDefendBonus / 2);
                    }

                    if (playerHPPercent <= 20) {
                        specialChance += 10;
                        defendChance -= 5;
                    }

                    if (defendChance > 85) defendChance = 85;
                    if (specialChance < 15) specialChance = 15;

                    int roll = generateRandomNumber(1, 100);
                    if (roll <= defendChance) {
                        action = 2;
                    } else {
                        action = 3;
                    }
                } else {
                    if (aiHPPercent >= 60) {
                        int defendChance = 80 + baseDefendBonus;
                        if (defendChance > 95) defendChance = 95;
                        if (generateRandomNumber(1, 100) <= defendChance) {
                            action = 2;
                        } else {
                            action = 1;
                        }
                    } else if (aiHPPercent >= 30) {
                        int defendChance = 65 + baseDefendBonus;
                        if (defendChance > 90) defendChance = 90;
                        if (generateRandomNumber(1, 100) <= defendChance) {
                            action = 2;
                        } else {
                            action = 1;
                        }
                    } else {
                        int defendChance = 40 + baseDefendBonus;
                        if (defendChance > 70) defendChance = 70;
                        if (generateRandomNumber(1, 100) <= defendChance) {
                            action = 2;
                        } else {
                            action = 1;
                        }
                    }
                }
                break;

            case 2:
                if (aiChakraLevel >= 100) {
                    int defendChance = 45 + (baseDefendBonus / 2);
                    int roll = generateRandomNumber(1, 100);
                    if (roll <= defendChance) {
                        action = 2;
                    } else {
                        action = 3;
                    }
                } else {
                    if (aiHPPercent >= 70) {
                        int defendChance = 60 + (baseDefendBonus / 2);
                        if (generateRandomNumber(1, 100) <= defendChance) {
                            action = 2;
                        } else {
                            action = 1;
                        }
                    } else {
                        if (playerCanSpecial && generateRandomNumber(1, 100) <= 30) {
                            action = 2;
                        } else {
                            action = generateRandomNumber(1, 2);
                        }
                    }
                }
                break;

            case 3:
                if (aiChakraLevel >= 100) {
                    int bossHPPercent = (bossCurrentHP * 100) / bossMaxHP;
                    bool playerCanSpecial = (playerChakraLevel >= 100);

                    if (bossHPPercent <= 30) {
                        if (generateRandomNumber(1, 100) <= 70) {
                            action = 3;
                        } else {
                            action = 2;
                        }
                    } else if (playerCanSpecial) {
                        if (generateRandomNumber(1, 100) <= 40) {
                            action = 2;
                        } else {
                            action = 3;
                        }
                    } else {
                        if (generateRandomNumber(1, 100) <= 50) {
                            action = 3;
                        } else {
                            action = 1;
                        }
                    }
                } else {
                    int bossHPPercent = (bossCurrentHP * 100) / bossMaxHP;
                    bool playerCanSpecial = (playerChakraLevel >= 100);

                    if (bossHPPercent <= 50) {
                        if (generateRandomNumber(1, 100) <= 70) {
                            action = 2;
                        } else {
                            action = 1;
                        }
                    } else if (playerCanSpecial) {
                        if (generateRandomNumber(1, 100) <= 60) {
                            action = 2;
                        } else {
                            action = 1;
                        }
                    } else {
                        if (generateRandomNumber(1, 100) <= 60) {
                            action = 1;
                        } else {
                            action = 2;
                        }
                    }
                }
                break;

            default:
                action = 1;
                break;
        }
    }

    switch(action)
    {
        case 1:
            if (!gameEnded && bossCurrentHP > 0) aiAttack();
            break;
        case 2:
            if (!gameEnded && bossCurrentHP > 0) aiDefend();
            break;
        case 3:
            if (!gameEnded && bossCurrentHP > 0 && aiChakraLevel >= 100) aiSpecial();
            break;
    }
}

void NarutoGameScreen2View::aiAttack()
{
    if (gameEnded) return;

    resetBleachState();

    bleach.setVisible(false);
    bleach_atk.setVisible(true);
    bleach_atk.invalidate();
    bleach.invalidate();

    int baseDamage;
    switch(bossPhase) {
        case 1:
            baseDamage = generateRandomNumber(4, 10);
            break;
        case 2:
            baseDamage = generateRandomNumber(6, 12);
            break;
        case 3:
            baseDamage = generateRandomNumber(8, 15);
            break;
        case 4:
            baseDamage = generateRandomNumber(10, 18);
            break;
        default:
            baseDamage = generateRandomNumber(4, 10);
            break;
    }

    bool playerHasShield = (playerPowerUpActive && playerPowerUpType == 4);
    int actualDamage = calculateDamage(baseDamage, playerDefending, false, playerHasShield);

    playerHP -= actualDamage;
    if (playerHP < 0) playerHP = 0;

    int chakraGain = generateRandomNumber(4, 8);
    aiChakraLevel += chakraGain;
    if (aiChakraLevel > 100) aiChakraLevel = 100;

    if (playerHasShield && actualDamage == 0) {
        clearPowerUpAfterUse(true);
    }

    updatePlayerHPDisplay();
    updateAIChakraDisplay();

    startAttackFireAnimation();
    startResetTimer();
}

void NarutoGameScreen2View::aiDefend()
{
    if (gameEnded) return;

    resetBleachState();

    bleach.setVisible(false);
    bleach_def.setVisible(true);
    bleach_def.invalidate();
    bleach.invalidate();

    aiDefending = true;

    int healAmount;
    switch(bossPhase) {
        case 1:
            healAmount = generateRandomNumber(2, 5);
            break;
        case 2:
            healAmount = generateRandomNumber(3, 6);
            break;
        case 3:
            healAmount = generateRandomNumber(4, 7);
            break;
        case 4:
            healAmount = generateRandomNumber(5, 8);
            break;
        default:
            healAmount = generateRandomNumber(2, 5);
            break;
    }

    bossCurrentHP += healAmount;
    if (bossCurrentHP > bossMaxHP) bossCurrentHP = bossMaxHP;

    aiChakraLevel += generateRandomNumber(8, 12);
    if (aiChakraLevel > 100) aiChakraLevel = 100;

    updateBossHPDisplay();
    updateAIChakraDisplay();

    aiActionDone = true;
}

void NarutoGameScreen2View::aiSpecial()
{
    int requiredChakra = 100;
    if (rageMode && bossPhase >= 3) {
        requiredChakra = 80;
    }

    if (aiChakraLevel < requiredChakra || gameEnded) {
        return;
    }

    resetBleachState();

    bleach.setVisible(false);
    bleach_spe.setVisible(true);
    bleach_spe.invalidate();
    bleach.invalidate();

    int specialDamage = 70;

    bool playerHasShield = (playerPowerUpActive && playerPowerUpType == 4);
    int actualDamage = calculateSpecialDamage(specialDamage, playerDefending, playerHasShield);

    playerHP -= actualDamage;
    if (playerHP < 0) playerHP = 0;

    aiChakraLevel = 0;

    if (playerHasShield && actualDamage == 0) {
        clearPowerUpAfterUse(true);
    }

    updateAIChakraDisplay();
    updatePlayerHPDisplay();

    startSpecialFireAnimation();
    startResetTimer();
}

void NarutoGameScreen2View::resetNarutoState()
{
    naruto_atk.setVisible(false);
    naruto_def.setVisible(false);
    naruto_spe.setVisible(false);
    naruto_lose.setVisible(false);
    naruto_win.setVisible(false);

    if (playerHP > 0 && !gameEnded) {
        naruto.setVisible(true);
    }

    naruto_atk.invalidate();
    naruto_def.invalidate();
    naruto_spe.invalidate();
    naruto_lose.invalidate();
    naruto_win.invalidate();
    naruto.invalidate();
}

void NarutoGameScreen2View::resetBleachState()
{
    bleach_atk.setVisible(false);
    bleach_def.setVisible(false);
    bleach_spe.setVisible(false);
    bleach_lose.setVisible(false);
    bleach_win.setVisible(false);

    if (bossCurrentHP > 0 && !gameEnded) {
        bleach.setVisible(true);
    }

    bleach_atk.invalidate();
    bleach_def.invalidate();
    bleach_spe.invalidate();
    bleach_lose.invalidate();
    bleach_win.invalidate();
    bleach.invalidate();
}

void NarutoGameScreen2View::startResetTimer()
{
    resetTimer = 0;
    resetTimerActive = true;
}

void NarutoGameScreen2View::updatePlayerHPDisplay()
{
    hideAllPlayerHPWidgets();

    int roundedHP = (playerHP / 10) * 10;

    if (roundedHP < 0) roundedHP = 0;
    if (roundedHP > 100) roundedHP = 100;

    if (playerHP == 0 || roundedHP == 0) {
        BITMAP_HP_0_ID.setVisible(true);
        BITMAP_HP_0_ID.invalidate();

        for (int i = 0; i < 10; i++) {
            toggleLED();
        }

        if (!gameEnded) {
            endGame(false);
        }
        return;
    }

    switch (roundedHP) {
        case 100:
            BITMAP_HP_100_ID.setVisible(true);
            BITMAP_HP_100_ID.invalidate();
            break;
        case 90:
            BITMAP_HP_90_ID.setVisible(true);
            BITMAP_HP_90_ID.invalidate();
            break;
        case 80:
            BITMAP_HP_80_ID.setVisible(true);
            BITMAP_HP_80_ID.invalidate();
            break;
        case 70:
            BITMAP_HP_70_ID.setVisible(true);
            BITMAP_HP_70_ID.invalidate();
            break;
        case 60:
            BITMAP_HP_60_ID.setVisible(true);
            BITMAP_HP_60_ID.invalidate();
            break;
        case 50:
            BITMAP_HP_50_ID.setVisible(true);
            BITMAP_HP_50_ID.invalidate();
            break;
        case 40:
            BITMAP_HP_40_ID.setVisible(true);
            BITMAP_HP_40_ID.invalidate();
            break;
        case 30:
            BITMAP_HP_30_ID.setVisible(true);
            BITMAP_HP_30_ID.invalidate();
            break;
        case 20:
            BITMAP_HP_20_ID.setVisible(true);
            BITMAP_HP_20_ID.invalidate();
            break;
        case 10:
            BITMAP_HP_10_ID.setVisible(true);
            BITMAP_HP_10_ID.invalidate();
            break;
        default:
            BITMAP_HP_0_ID.setVisible(true);
            BITMAP_HP_0_ID.invalidate();
            if (!gameEnded) {
                endGame(false);
            }
            break;
    }
}

void NarutoGameScreen2View::hideAllPlayerHPWidgets()
{
    BITMAP_HP_100_ID.setVisible(false);
    BITMAP_HP_90_ID.setVisible(false);
    BITMAP_HP_80_ID.setVisible(false);
    BITMAP_HP_70_ID.setVisible(false);
    BITMAP_HP_60_ID.setVisible(false);
    BITMAP_HP_50_ID.setVisible(false);
    BITMAP_HP_40_ID.setVisible(false);
    BITMAP_HP_30_ID.setVisible(false);
    BITMAP_HP_20_ID.setVisible(false);
    BITMAP_HP_10_ID.setVisible(false);
    BITMAP_HP_0_ID.setVisible(false);

    BITMAP_HP_100_ID.invalidate();
    BITMAP_HP_90_ID.invalidate();
    BITMAP_HP_80_ID.invalidate();
    BITMAP_HP_70_ID.invalidate();
    BITMAP_HP_60_ID.invalidate();
    BITMAP_HP_50_ID.invalidate();
    BITMAP_HP_40_ID.invalidate();
    BITMAP_HP_30_ID.invalidate();
    BITMAP_HP_20_ID.invalidate();
    BITMAP_HP_10_ID.invalidate();
    BITMAP_HP_0_ID.invalidate();
}

int NarutoGameScreen2View::calculateSpecialDamage(int baseDamage, bool isDefending, bool hasShield)
{
    if (hasShield) {
        return 0;
    }

    int finalDamage = baseDamage;

    if (isDefending) {
        finalDamage = finalDamage / 2;

        if (finalDamage < 1) finalDamage = 1;
    }

    return finalDamage;
}

void NarutoGameScreen2View::updatePlayerChakraDisplay()
{
    if (playerChakraLevel < 0) playerChakraLevel = 0;
    if (playerChakraLevel > 100) playerChakraLevel = 100;

    if (playerChakraLevel == 0) {
        chakra_box1.setVisible(false);
        chakra_box1.invalidate();
        return;
    }

    chakra_box1.setVisible(true);

    int maxWidth = 86;
    int currentWidth = (playerChakraLevel * maxWidth) / 100;

    if (currentWidth < 1) currentWidth = 1;
    if (currentWidth > maxWidth) currentWidth = maxWidth;

    chakra_box1.setPosition(14, 43, currentWidth, 5);
    chakra_box1.invalidate();
}

void NarutoGameScreen2View::updateAIChakraDisplay()
{
    if (aiChakraLevel < 0) aiChakraLevel = 0;
    if (aiChakraLevel > 100) aiChakraLevel = 100;

    if (aiChakraLevel == 0) {
        chakra_box2.setVisible(false);
        chakra_box2.invalidate();
        return;
    }

    chakra_box2.setVisible(true);

    int maxWidth = 86;
    int currentWidth = (aiChakraLevel * maxWidth) / 100;

    if (currentWidth < 1) currentWidth = 1;
    if (currentWidth > maxWidth) currentWidth = maxWidth;

    chakra_box2.setPosition(135, 43, currentWidth, 5);
    chakra_box2.invalidate();
}

void NarutoGameScreen2View::gainChakra(int amount)
{
    playerChakraLevel += amount;
    if (playerChakraLevel > 100)
    {
        playerChakraLevel = 100;
    }

    updatePlayerChakraDisplay();
}

void NarutoGameScreen2View::useChakra(int amount)
{
    playerChakraLevel -= amount;
    if (playerChakraLevel < 0)
    {
        playerChakraLevel = 0;
    }

    updatePlayerChakraDisplay();
}

void NarutoGameScreen2View::checkGameEnd()
{
    return;
}

void NarutoGameScreen2View::endGame(bool playerWon)
{
    gameEnded = true;

    hideActionButtons();
    btn_arrow1.setVisible(false);
    btn_arrow1.invalidate();

    hideAllPlayerPowerUps();

    fireAnimationActive = false;
    fireAnimationTimer = 0;
    resetTimerActive = false;
    resetTimer = 0;

    hideAllFireEffects();

    naruto.setVisible(false);
    naruto_atk.setVisible(false);
    naruto_def.setVisible(false);
    naruto_spe.setVisible(false);
    naruto_lose.setVisible(false);
    naruto_win.setVisible(false);

    bleach.setVisible(false);
    bleach_atk.setVisible(false);
    bleach_def.setVisible(false);
    bleach_spe.setVisible(false);
    bleach_lose.setVisible(false);
    bleach_win.setVisible(false);

    gameover.setVisible(false);
    win.setVisible(false);

    naruto.invalidate();
    naruto_atk.invalidate();
    naruto_def.invalidate();
    naruto_spe.invalidate();
    naruto_lose.invalidate();
    naruto_win.invalidate();

    bleach.invalidate();
    bleach_atk.invalidate();
    bleach_def.invalidate();
    bleach_spe.invalidate();
    bleach_lose.invalidate();
    bleach_win.invalidate();

    if (playerWon) {
        for (int i = 0; i < 8; i++) {
            toggleLED();
        }

        naruto_win.setVisible(true);
        bleach_lose.setVisible(true);
        win.setVisible(true);

        naruto_win.invalidate();
        bleach_lose.invalidate();
        win.invalidate();

        for (int i = 0; i < 6; i++) {
            toggleLED();
        }
    } else {
        naruto_lose.setVisible(true);
        bleach_win.setVisible(true);
        gameover.setVisible(true);

        naruto_lose.invalidate();
        bleach_win.invalidate();
        gameover.invalidate();

        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);

        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);

        application().gotoBackUpScreenScreenNoTransition();
        return;
    }

    autoReturnTimer = 0;
    autoReturnTimerActive = true;

    toggleLED();
    toggleLED();
    toggleLED();

    gameEnded = true;
}

void NarutoGameScreen2View::activateEasterEgg()
{
    easterEggActivated = true;
    easterEggTimer = 0;
    gameEnded = true;

    fireAnimationActive = false;
    fireAnimationTimer = 0;
    resetTimerActive = false;
    resetTimer = 0;

    hideAllFireEffects();

    hideActionButtons();
    btn_arrow1.setVisible(false);
    btn_arrow1.invalidate();

    hideAllPlayerPowerUps();

    naruto.setVisible(false);
    naruto_atk.setVisible(false);
    naruto_def.setVisible(false);
    naruto_spe.setVisible(false);
    naruto_lose.setVisible(false);
    naruto_win.setVisible(false);

    bleach.setVisible(false);
    bleach_atk.setVisible(false);
    bleach_def.setVisible(false);
    bleach_spe.setVisible(false);
    bleach_lose.setVisible(false);
    bleach_win.setVisible(false);

    gameover.setVisible(false);
    win.setVisible(false);

    naruto.invalidate();
    naruto_atk.invalidate();
    naruto_def.invalidate();
    naruto_spe.invalidate();
    naruto_lose.invalidate();
    naruto_win.invalidate();

    bleach.invalidate();
    bleach_atk.invalidate();
    bleach_def.invalidate();
    bleach_spe.invalidate();
    bleach_lose.invalidate();
    bleach_win.invalidate();

    naruto_win.setVisible(true);
    bleach_lose.setVisible(true);
    win.setVisible(true);

    naruto_win.invalidate();
    bleach_lose.invalidate();
    win.invalidate();

    for (int i = 0; i < 15; i++) {
        toggleLED();
    }

    toggleLED();
    toggleLED();
    toggleLED();
}

void NarutoGameScreen2View::updateBossHPDisplay()
{
    hideAllBossHPWidgets();

    int roundedHP = (bossCurrentHP / 10) * 10;
    if (roundedHP < 0) roundedHP = 0;
    if (roundedHP > 130) roundedHP = 130;

    if (bossCurrentHP == 0 || roundedHP == 0) {
        BITMAP_HP_0_ID_1.setVisible(true);
        BITMAP_HP_0_ID_1.invalidate();

        for (int i = 0; i < 10; i++) {
            toggleLED();
        }

        if (!gameEnded) {
            endGame(true);
        }
        return;
    }

    switch (roundedHP) {
        case 130:
            BITMAP_HP_130_ID_1.setVisible(true);
            BITMAP_HP_130_ID_1.invalidate();
            break;
        case 120:
            BITMAP_HP_120_ID_1.setVisible(true);
            BITMAP_HP_120_ID_1.invalidate();
            break;
        case 110:
            BITMAP_HP_110_ID_1.setVisible(true);
            BITMAP_HP_110_ID_1.invalidate();
            break;
        case 100:
            BITMAP_HP_100_ID_1.setVisible(true);
            BITMAP_HP_100_ID_1.invalidate();
            break;
        case 90:
            BITMAP_HP_90_ID_1.setVisible(true);
            BITMAP_HP_90_ID_1.invalidate();
            break;
        case 80:
            BITMAP_HP_80_ID_1.setVisible(true);
            BITMAP_HP_80_ID_1.invalidate();
            break;
        case 70:
            BITMAP_HP_70_ID_1.setVisible(true);
            BITMAP_HP_70_ID_1.invalidate();
            break;
        case 60:
            BITMAP_HP_60_ID_1.setVisible(true);
            BITMAP_HP_60_ID_1.invalidate();
            break;
        case 50:
            BITMAP_HP_50_ID_1.setVisible(true);
            BITMAP_HP_50_ID_1.invalidate();
            break;
        case 40:
            BITMAP_HP_40_ID_1.setVisible(true);
            BITMAP_HP_40_ID_1.invalidate();
            break;
        case 30:
            BITMAP_HP_30_ID_1.setVisible(true);
            BITMAP_HP_30_ID_1.invalidate();
            break;
        case 20:
            BITMAP_HP_20_ID_1.setVisible(true);
            BITMAP_HP_20_ID_1.invalidate();
            break;
        case 10:
            BITMAP_HP_10_ID_1.setVisible(true);
            BITMAP_HP_10_ID_1.invalidate();
            break;
        default:
            BITMAP_HP_0_ID_1.setVisible(true);
            BITMAP_HP_0_ID_1.invalidate();
            if (!gameEnded) {
                endGame(true);
            }
            break;
    }
}
