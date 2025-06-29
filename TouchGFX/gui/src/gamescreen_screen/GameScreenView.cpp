#include <gui/gamescreen_screen/GameScreenView.hpp>
#include <touchgfx/Color.hpp>
#include "stm32f4xx_hal.h"
#include <gui/character_screen/CharacterView.hpp>

GameScreenView::GameScreenView()
{
    resetTimer = 0;
    resetTimerActive = false;
    randomSeed = 12345;

    damageDisplayTimer = 0;
    damageDisplayActive = false;
    currentDamageAmount = 0;
    isPlayerDamage = false;
    damageTextStartY = 99;
    damageTextAIStartY = 141;
}

void GameScreenView::setupScreen()
{
    GameScreenViewBase::setupScreen();
    initGPIO();

    randomSeed = HAL_GetTick();
    if (randomSeed == 0) randomSeed = 12345;
    randomSeed = (randomSeed * 1664525 + 1013904223);

    playerHP = 100;
    aiHP = 100;
    playerChakraLevel = 0;
    aiChakraLevel = 0;
    menuOpen = false;
    gameEnded = false;
    playerWonGame = false;

    playerDefending = false;
    aiDefending = false;
    playerSpecialUsed = false;
    aiSpecialUsed = false;

    turnCounter = 0;
    playerPowerUpType = 0;
    aiPowerUpType = 0;
    playerPowerUpActive = false;
    aiPowerUpActive = false;
    playerActionDone = false;
    aiActionDone = false;

    hideDamageText();

    damageDisplayTimer = 0;
    damageDisplayActive = false;
    currentDamageAmount = 0;
    isPlayerDamage = false;

    damageTextStartY = 99;
    damageTextAIStartY = 141;

    int personalityRoll = generateRandomNumber(1, 100);
    if (personalityRoll <= 70) {
        aiPersonality = 0;
    } else if (personalityRoll <= 90) {
        aiPersonality = 1;
    } else {
        aiPersonality = 2;
    }

    resetTimer = 0;
    resetTimerActive = false;
    autoReturnTimer = 0;
    autoReturnTimerActive = false;

    hideAllPlayerPowerUps();
    hideAllAIPowerUps();

    updatePlayerHPDisplay();
    updateAIHPDisplay();
    updatePlayerChakraDisplay();
    updateAIChakraDisplay();

    resetNarutoState();
    resetBleachState();
}

void GameScreenView::showDamageText(int playerDamage, int aiDamage)
{
    hideDamageText();

    currentPlayerDamage = playerDamage;
    currentAIDamage = aiDamage;
    playerDamageActive = (playerDamage > 0);
    aiDamageActive = (aiDamage > 0);

    if (playerDamage > 0) {
        switch(playerDamage) {
            case 0:
                minus_0.setVisible(true);
                minus_0.setAlpha(255);
                minus_0.invalidate();
                break;
            case 1:
            case 2:
                minus_1.setVisible(true);
                minus_1.setAlpha(255);
                minus_1.invalidate();
                break;
            case 3:
                minus_3.setVisible(true);
                minus_3.setAlpha(255);
                minus_3.invalidate();
                break;
            case 4:
                minus_4.setVisible(true);
                minus_4.setAlpha(255);
                minus_4.invalidate();
                break;
            case 5:
                minus_5.setVisible(true);
                minus_5.setAlpha(255);
                minus_5.invalidate();
                break;
            case 6:
                minus_6.setVisible(true);
                minus_6.setAlpha(255);
                minus_6.invalidate();
                break;
            case 7:
                minus_7.setVisible(true);
                minus_7.setAlpha(255);
                minus_7.invalidate();
                break;
            case 8:
                minus_8.setVisible(true);
                minus_8.setAlpha(255);
                minus_8.invalidate();
                break;
            case 10:
                minus_10.setVisible(true);
                minus_10.setAlpha(255);
                minus_10.invalidate();
                break;
            case 12:
                minus_12.setVisible(true);
                minus_12.setAlpha(255);
                minus_12.invalidate();
                break;
            case 15:
                minus_15.setVisible(true);
                minus_15.setAlpha(255);
                minus_15.invalidate();
                break;
            case 17:
                minus_17.setVisible(true);
                minus_17.setAlpha(255);
                minus_17.invalidate();
                break;
            case 20:
                minus_20.setVisible(true);
                minus_20.setAlpha(255);
                minus_20.invalidate();
                break;
            case 25:
                minus_25.setVisible(true);
                minus_25.setAlpha(255);
                minus_25.invalidate();
                break;
            case 40:
                minus_40.setVisible(true);
                minus_40.setAlpha(255);
                minus_40.invalidate();
                break;
            case 100:
                minus_100.setVisible(true);
                minus_100.setAlpha(255);
                minus_100.invalidate();
                break;
            default:
                if (playerDamage >= 90) {
                    minus_100.setVisible(true);
                    minus_100.setAlpha(255);
                    minus_100.invalidate();
                } else if (playerDamage >= 35) {
                    minus_40.setVisible(true);
                    minus_40.setAlpha(255);
                    minus_40.invalidate();
                } else if (playerDamage >= 22) {
                    minus_25.setVisible(true);
                    minus_25.setAlpha(255);
                    minus_25.invalidate();
                } else if (playerDamage >= 18) {
                    minus_20.setVisible(true);
                    minus_20.setAlpha(255);
                    minus_20.invalidate();
                } else if (playerDamage >= 9) {
                    minus_10.setVisible(true);
                    minus_10.setAlpha(255);
                    minus_10.invalidate();
                } else {
                    minus_8.setVisible(true);
                    minus_8.setAlpha(255);
                    minus_8.invalidate();
                }
                break;
        }
        toggleLED();
    }

    if (aiDamage > 0) {
        switch(aiDamage) {
            case 0:
                minus_0_1.setVisible(true);
                minus_0_1.setAlpha(255);
                minus_0_1.invalidate();
                break;
            case 1:
            case 2:
                minus_1_1.setVisible(true);
                minus_1_1.setAlpha(255);
                minus_1_1.invalidate();
                break;
            case 3:
                minus_3_1.setVisible(true);
                minus_3_1.setAlpha(255);
                minus_3_1.invalidate();
                break;
            case 4:
                minus_4_1.setVisible(true);
                minus_4_1.setAlpha(255);
                minus_4_1.invalidate();
                break;
            case 5:
                minus_5_1.setVisible(true);
                minus_5_1.setAlpha(255);
                minus_5_1.invalidate();
                break;
            case 6:
                minus_6_1.setVisible(true);
                minus_6_1.setAlpha(255);
                minus_6_1.invalidate();
                break;
            case 7:
                minus_7_1.setVisible(true);
                minus_7_1.setAlpha(255);
                minus_7_1.invalidate();
                break;
            case 8:
                minus_8_1.setVisible(true);
                minus_8_1.setAlpha(255);
                minus_8_1.invalidate();
                break;
            case 10:
                minus_10_1.setVisible(true);
                minus_10_1.setAlpha(255);
                minus_10_1.invalidate();
                break;
            case 12:
                minus_12_1.setVisible(true);
                minus_12_1.setAlpha(255);
                minus_12_1.invalidate();
                break;
            case 15:
                minus_15_1.setVisible(true);
                minus_15_1.setAlpha(255);
                minus_15_1.invalidate();
                break;
            case 17:
                minus_17_1.setVisible(true);
                minus_17_1.setAlpha(255);
                minus_17_1.invalidate();
                break;
            case 20:
                minus_20_1.setVisible(true);
                minus_20_1.setAlpha(255);
                minus_20_1.invalidate();
                break;
            case 25:
                minus_25_1.setVisible(true);
                minus_25_1.setAlpha(255);
                minus_25_1.invalidate();
                break;
            case 40:
                minus_40_1.setVisible(true);
                minus_40_1.setAlpha(255);
                minus_40_1.invalidate();
                break;
            case 100:
                minus_100_1.setVisible(true);
                minus_100_1.setAlpha(255);
                minus_100_1.invalidate();
                break;
            default:
                if (aiDamage >= 90) {
                    minus_100_1.setVisible(true);
                    minus_100_1.setAlpha(255);
                    minus_100_1.invalidate();
                } else if (aiDamage >= 35) {
                    minus_40_1.setVisible(true);
                    minus_40_1.setAlpha(255);
                    minus_40_1.invalidate();
                } else if (aiDamage >= 22) {
                    minus_25_1.setVisible(true);
                    minus_25_1.setAlpha(255);
                    minus_25_1.invalidate();
                } else if (aiDamage >= 18) {
                    minus_20_1.setVisible(true);
                    minus_20_1.setAlpha(255);
                    minus_20_1.invalidate();
                } else if (aiDamage >= 9) {
                    minus_10_1.setVisible(true);
                    minus_10_1.setAlpha(255);
                    minus_10_1.invalidate();
                } else {
                    minus_8_1.setVisible(true);
                    minus_8_1.setAlpha(255);
                    minus_8_1.invalidate();
                }
                break;
        }
        toggleLED();
        toggleLED();
    }

    if (playerDamage > 0 || aiDamage > 0) {
        damageDisplayTimer = 0;
        damageDisplayActive = true;

        if (playerDamage > 0 && aiDamage > 0) {
            toggleLED();
            toggleLED();
            toggleLED();
        }
    }
}

void GameScreenView::hideDamageText()
{
    minus_0.setVisible(false);
    minus_1.setVisible(false);
    minus_3.setVisible(false);
    minus_4.setVisible(false);
    minus_5.setVisible(false);
    minus_6.setVisible(false);
    minus_7.setVisible(false);
    minus_8.setVisible(false);
    minus_10.setVisible(false);
    minus_12.setVisible(false);
    minus_15.setVisible(false);
    minus_17.setVisible(false);
    minus_20.setVisible(false);
    minus_25.setVisible(false);
    minus_40.setVisible(false);
    minus_100.setVisible(false);

    minus_0_1.setVisible(false);
    minus_1_1.setVisible(false);
    minus_3_1.setVisible(false);
    minus_4_1.setVisible(false);
    minus_5_1.setVisible(false);
    minus_6_1.setVisible(false);
    minus_7_1.setVisible(false);
    minus_8_1.setVisible(false);
    minus_10_1.setVisible(false);
    minus_12_1.setVisible(false);
    minus_15_1.setVisible(false);
    minus_17_1.setVisible(false);
    minus_20_1.setVisible(false);
    minus_25_1.setVisible(false);
    minus_40_1.setVisible(false);
    minus_100_1.setVisible(false);

    minus_0.invalidate(); minus_1.invalidate(); minus_3.invalidate();
    minus_4.invalidate(); minus_5.invalidate(); minus_6.invalidate();
    minus_7.invalidate(); minus_8.invalidate(); minus_10.invalidate();
    minus_12.invalidate(); minus_15.invalidate(); minus_17.invalidate();
    minus_20.invalidate(); minus_25.invalidate(); minus_40.invalidate();
    minus_100.invalidate();

    minus_0_1.invalidate(); minus_1_1.invalidate(); minus_3_1.invalidate();
    minus_4_1.invalidate(); minus_5_1.invalidate(); minus_6_1.invalidate();
    minus_7_1.invalidate(); minus_8_1.invalidate(); minus_10_1.invalidate();
    minus_12_1.invalidate(); minus_15_1.invalidate(); minus_17_1.invalidate();
    minus_20_1.invalidate(); minus_25_1.invalidate(); minus_40_1.invalidate();
    minus_100_1.invalidate();

    damageDisplayActive = false;
    damageDisplayTimer = 0;
    playerDamageActive = false;
    aiDamageActive = false;
    currentPlayerDamage = 0;
    currentAIDamage = 0;
}

void GameScreenView::updateDamageDisplay()
{
    if (!damageDisplayActive) return;

    damageDisplayTimer += 16;
    float progress = (float)damageDisplayTimer / DAMAGE_DISPLAY_TIME_MS;
    if (progress > 1.0f) progress = 1.0f;

    int alpha = 255 - (int)(progress * 255);
    if (alpha < 0) alpha = 0;

    if (playerDamageActive && currentPlayerDamage > 0) {
        switch(currentPlayerDamage) {
            case 0: minus_0.setAlpha(alpha); minus_0.invalidate(); break;
            case 1:
            case 2: minus_1.setAlpha(alpha); minus_1.invalidate(); break;
            case 3: minus_3.setAlpha(alpha); minus_3.invalidate(); break;
            case 4: minus_4.setAlpha(alpha); minus_4.invalidate(); break;
            case 5: minus_5.setAlpha(alpha); minus_5.invalidate(); break;
            case 6: minus_6.setAlpha(alpha); minus_6.invalidate(); break;
            case 7: minus_7.setAlpha(alpha); minus_7.invalidate(); break;
            case 8: minus_8.setAlpha(alpha); minus_8.invalidate(); break;
            case 10: minus_10.setAlpha(alpha); minus_10.invalidate(); break;
            case 12: minus_12.setAlpha(alpha); minus_12.invalidate(); break;
            case 15: minus_15.setAlpha(alpha); minus_15.invalidate(); break;
            case 17: minus_17.setAlpha(alpha); minus_17.invalidate(); break;
            case 20: minus_20.setAlpha(alpha); minus_20.invalidate(); break;
            case 25: minus_25.setAlpha(alpha); minus_25.invalidate(); break;
            case 40: minus_40.setAlpha(alpha); minus_40.invalidate(); break;
            case 100: minus_100.setAlpha(alpha); minus_100.invalidate(); break;
            default:
                if (currentPlayerDamage >= 90) {
                    minus_100.setAlpha(alpha); minus_100.invalidate();
                } else if (currentPlayerDamage >= 35) {
                    minus_40.setAlpha(alpha); minus_40.invalidate();
                } else if (currentPlayerDamage >= 22) {
                    minus_25.setAlpha(alpha); minus_25.invalidate();
                } else if (currentPlayerDamage >= 18) {
                    minus_20.setAlpha(alpha); minus_20.invalidate();
                } else if (currentPlayerDamage >= 9) {
                    minus_10.setAlpha(alpha); minus_10.invalidate();
                } else {
                    minus_8.setAlpha(alpha); minus_8.invalidate();
                }
                break;
        }
    }

    if (aiDamageActive && currentAIDamage > 0) {
        switch(currentAIDamage) {
            case 0: minus_0_1.setAlpha(alpha); minus_0_1.invalidate(); break;
            case 1:
            case 2: minus_1_1.setAlpha(alpha); minus_1_1.invalidate(); break;
            case 3: minus_3_1.setAlpha(alpha); minus_3_1.invalidate(); break;
            case 4: minus_4_1.setAlpha(alpha); minus_4_1.invalidate(); break;
            case 5: minus_5_1.setAlpha(alpha); minus_5_1.invalidate(); break;
            case 6: minus_6_1.setAlpha(alpha); minus_6_1.invalidate(); break;
            case 7: minus_7_1.setAlpha(alpha); minus_7_1.invalidate(); break;
            case 8: minus_8_1.setAlpha(alpha); minus_8_1.invalidate(); break;
            case 10: minus_10_1.setAlpha(alpha); minus_10_1.invalidate(); break;
            case 12: minus_12_1.setAlpha(alpha); minus_12_1.invalidate(); break;
            case 15: minus_15_1.setAlpha(alpha); minus_15_1.invalidate(); break;
            case 17: minus_17_1.setAlpha(alpha); minus_17_1.invalidate(); break;
            case 20: minus_20_1.setAlpha(alpha); minus_20_1.invalidate(); break;
            case 25: minus_25_1.setAlpha(alpha); minus_25_1.invalidate(); break;
            case 40: minus_40_1.setAlpha(alpha); minus_40_1.invalidate(); break;
            case 100: minus_100_1.setAlpha(alpha); minus_100_1.invalidate(); break;
            default:
                if (currentAIDamage >= 90) {
                    minus_100_1.setAlpha(alpha); minus_100_1.invalidate();
                } else if (currentAIDamage >= 35) {
                    minus_40_1.setAlpha(alpha); minus_40_1.invalidate();
                } else if (currentAIDamage >= 22) {
                    minus_25_1.setAlpha(alpha); minus_25_1.invalidate();
                } else if (currentAIDamage >= 18) {
                    minus_20_1.setAlpha(alpha); minus_20_1.invalidate();
                } else if (currentAIDamage >= 9) {
                    minus_10_1.setAlpha(alpha); minus_10_1.invalidate();
                } else {
                    minus_8_1.setAlpha(alpha); minus_8_1.invalidate();
                }
                break;
        }
    }

    if (damageDisplayTimer >= DAMAGE_DISPLAY_TIME_MS) {
        hideDamageText();
    }
}

void GameScreenView::tearDownScreen()
{
    GameScreenViewBase::tearDownScreen();
}

void GameScreenView::initGPIO()
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

void GameScreenView::toggleLED()
{
    HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
}

void GameScreenView::hideActionButtons()
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

void GameScreenView::showActionButtons()
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

void GameScreenView::toggleActionMenu()
{
    if (menuOpen) {
        hideActionButtons();
    } else {
        showActionButtons();
    }
    toggleLED();
}

void GameScreenView::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        if (!gameEnded) {
            bool clickedOnBleach = (evt.getX() >= 160 && evt.getX() <= 240 &&
                                   evt.getY() >= 140 && evt.getY() <= 280);

            bool clickedOnBleachSprite = false;

            if (bleach.isVisible() && bleach.getAbsoluteRect().intersect(evt.getX(), evt.getY())) {
                clickedOnBleachSprite = true;
            }
            if (bleach_atk.isVisible() && bleach_atk.getAbsoluteRect().intersect(evt.getX(), evt.getY())) {
                clickedOnBleachSprite = true;
            }
            if (bleach_def.isVisible() && bleach_def.getAbsoluteRect().intersect(evt.getX(), evt.getY())) {
                clickedOnBleachSprite = true;
            }
            if (bleach_spe.isVisible() && bleach_spe.getAbsoluteRect().intersect(evt.getX(), evt.getY())) {
                clickedOnBleachSprite = true;
            }

            if (clickedOnBleach || clickedOnBleachSprite) {
                for (int i = 0; i < 20; i++) {
                    toggleLED();
                }

                aiHP = 0;

                showDamageText(0, 999);

                resetNarutoState();
                resetBleachState();

                updateAIHPDisplay();

                for (int i = 0; i < 25; i++) {
                    toggleLED();
                }

                hideActionButtons();

                return;
            }
        }

        if (evt.getX() < 50 && evt.getY() < 50) {
            aiHP = 0;
            updateAIHPDisplay();
            return;
        }

        if (evt.getX() > 190 && evt.getY() < 50) {
            playerHP = 0;
            updatePlayerHPDisplay();
            return;
        }

        if (evt.getX() < 50 && evt.getY() > 270) {
            debugGameState();
            return;
        }

        if (evt.getX() > 190 && evt.getY() > 270 && !gameEnded) {
            giveRandomPowerUps();
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
            if (btn_arrow1.getAbsoluteRect().intersect(evt.getX(), evt.getY()))
            {
                toggleActionMenu();
                return;
            }

            if (btn_arrow2.getAbsoluteRect().intersect(evt.getX(), evt.getY()))
            {
                toggleActionMenu();
                return;
            }

            if (menuOpen) {
                if (btn_attack.getAbsoluteRect().intersect(evt.getX(), evt.getY()))
                {
                    playerAttack();
                    if (!gameEnded) performAIAction();
                    return;
                }
                else if (btn_def.getAbsoluteRect().intersect(evt.getX(), evt.getY()))
                {
                    playerDefend();
                    if (!gameEnded) performAIAction();
                    return;
                }
                else if (btn_spe.getAbsoluteRect().intersect(evt.getX(), evt.getY()))
                {
                    if (playerChakraLevel >= 100) {
                        playerSpecial();
                        if (!gameEnded) performAIAction();
                    }
                    return;
                }
            }
        }
    }

    GameScreenViewBase::handleClickEvent(evt);
}

void GameScreenView::handleDragEvent(const DragEvent& evt)
{
    GameScreenViewBase::handleDragEvent(evt);
}

void GameScreenView::handleTickEvent()
{
    if (!gameEnded) {
        if (resetTimerActive)
        {
            resetTimer += 16;
            if (resetTimer >= RESET_TIME_MS)
            {
                resetNarutoState();
                resetBleachState();
                resetTimerActive = false;
                resetTimer = 0;
                playerDefending = false;
                aiDefending = false;
                checkTurnComplete();
            }
        }

        updateDamageDisplay();
    }
    else {
        if (autoReturnTimerActive) {
            autoReturnTimer += 16;

            if (autoReturnTimer >= AUTO_RETURN_TIME_MS) {
                HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);

                if (playerWonGame) {
                    application().gotoBackUpScreenScreenNoTransition();
                } else {
                    application().gotoMainMenuScreenScreenNoTransition();
                }
                return;
            }
        }
    }

    GameScreenViewBase::handleTickEvent();
}

void GameScreenView::giveRandomPowerUpsHighHealChakra()
{
    for (int i = 0; i < 8; i++) {
        toggleLED();
    }

    int playerRoll = generateRandomNumber(1, 100);
    int newPlayerPowerUp;

    if (playerRoll >= 1 && playerRoll <= 40) {
        newPlayerPowerUp = 2;
    } else if (playerRoll >= 41 && playerRoll <= 75) {
        newPlayerPowerUp = 3;
    } else if (playerRoll >= 76 && playerRoll <= 90) {
        newPlayerPowerUp = 1;
    } else {
        newPlayerPowerUp = 4;
    }

    playerPowerUpType = newPlayerPowerUp;
    playerPowerUpActive = true;
    applyInstantPowerUp(true, playerPowerUpType);
    updatePlayerPowerUpDisplay();

    int aiRoll = generateRandomNumber(1, 100);
    int newAIPowerUp;

    if (aiRoll >= 1 && aiRoll <= 40) {
          newAIPowerUp = 2;
      } else if (aiRoll >= 41 && aiRoll <= 70) {
          newAIPowerUp = 3;
      } else if (aiRoll >= 71 && aiRoll <= 85) {
          newAIPowerUp = 1;
      } else {
          newAIPowerUp = 4;
      }

    aiPowerUpType = newAIPowerUp;
    aiPowerUpActive = true;
    applyInstantPowerUp(false, aiPowerUpType);
    updateAIPowerUpDisplay();

    for (int i = 0; i < playerPowerUpType; i++) {
        toggleLED();
    }

    for (int pause = 0; pause < 1000; pause++) {
    }

    for (int i = 0; i < aiPowerUpType + 5; i++) {
        toggleLED();
    }
}

void GameScreenView::debugGameState()
{
    if (gameEnded) {
        for (int i = 0; i < 10; i++) {
            toggleLED();
        }
    } else {
        int blinkCount = 1;

        if (playerPowerUpActive && aiPowerUpActive) {
            blinkCount = 4;
        } else if (playerPowerUpActive) {
            blinkCount = 2;
        } else if (aiPowerUpActive) {
            blinkCount = 3;
        }

        for (int i = 0; i < blinkCount; i++) {
            toggleLED();
        }
    }
}

void GameScreenView::checkTurnComplete()
{
    if (playerActionDone && aiActionDone) {
        turnCounter++;

        toggleLED();

        if (!playerSpecialUsed && !aiSpecialUsed) {
            int powerUpChance = generateRandomNumber(1, 100);
            if (powerUpChance <= 20) {
                giveRandomPowerUps();
            }
        }

        if (playerPowerUpActive && (playerPowerUpType == 2 || playerPowerUpType == 3 || playerPowerUpType == 4)) {
            clearPowerUpAfterUse(true);
        }
        if (aiPowerUpActive && (aiPowerUpType == 2 || aiPowerUpType == 3 || aiPowerUpType == 4)) {
            clearPowerUpAfterUse(false);
        }

        playerActionDone = false;
        aiActionDone = false;
        playerSpecialUsed = false;
        aiSpecialUsed = false;
    }
}

void GameScreenView::giveRandomPowerUps()
{
    for (int i = 0; i < 3; i++) {
        toggleLED();
    }

    int playerRoll = generateRandomNumber(1, 100);
    int newPlayerPowerUp;

    if (playerRoll >= 1 && playerRoll <= 35) {
        newPlayerPowerUp = 4;
    } else if (playerRoll >= 36 && playerRoll <= 65) {
        newPlayerPowerUp = 1;
    } else if (playerRoll >= 66 && playerRoll <= 85) {
        newPlayerPowerUp = 2;
    } else {
        newPlayerPowerUp = 3;
    }

    playerPowerUpType = newPlayerPowerUp;
    playerPowerUpActive = true;
    applyInstantPowerUp(true, playerPowerUpType);
    updatePlayerPowerUpDisplay();

    int aiRoll = generateRandomNumber(1, 100);
    int newAIPowerUp;

    if (aiRoll >= 1 && aiRoll <= 35) {
        newAIPowerUp = 4;
    } else if (aiRoll >= 36 && aiRoll <= 65) {
        newAIPowerUp = 1;
    } else if (aiRoll >= 66 && aiRoll <= 85) {
        newAIPowerUp = 2;
    } else {
        newAIPowerUp = 3;
    }

    aiPowerUpType = newAIPowerUp;
    aiPowerUpActive = true;
    applyInstantPowerUp(false, aiPowerUpType);
    updateAIPowerUpDisplay();
}

void GameScreenView::giveRandomPowerUpsAlternative()
{
    static int playerRotation = 1;
    static int aiRotation = 1;

    for (int i = 0; i < 8; i++) {
        toggleLED();
    }

    playerPowerUpType = playerRotation;
    playerPowerUpActive = true;
    applyInstantPowerUp(true, playerPowerUpType);
    updatePlayerPowerUpDisplay();

    playerRotation++;
    if (playerRotation > 4) playerRotation = 1;

    aiPowerUpType = aiRotation;
    aiPowerUpActive = true;
    applyInstantPowerUp(false, aiPowerUpType);
    updateAIPowerUpDisplay();

    aiRotation++;
    if (aiRotation > 4) aiRotation = 1;

    for (int i = 0; i < playerPowerUpType; i++) {
        toggleLED();
    }

    for (int pause = 0; pause < 1000; pause++) {
    }

    for (int i = 0; i < aiPowerUpType + 5; i++) {
        toggleLED();
    }
}

void GameScreenView::applyInstantPowerUp(bool isPlayer, int type)
{
    switch (type) {
        case 1:
            break;

        case 2:
            if (isPlayer) {
                playerHP += 5;
                if (playerHP > 100) playerHP = 100;
                updatePlayerHPDisplay();
            } else {
                aiHP += 5;
                if (aiHP > 100) aiHP = 100;
                updateAIHPDisplay();
            }
            break;

        case 3:
            if (isPlayer) {
                playerChakraLevel += 15;
                if (playerChakraLevel > 100) playerChakraLevel = 100;
                updatePlayerChakraDisplay();
            } else {
                aiChakraLevel += 15;
                if (aiChakraLevel > 100) aiChakraLevel = 100;
                updateAIChakraDisplay();
            }
            break;

        case 4:
            break;
    }
}

void GameScreenView::updatePlayerPowerUpDisplay()
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

void GameScreenView::updateAIPowerUpDisplay()
{
    hideAllAIPowerUps();

    if (!aiPowerUpActive) {
        return;
    }

    switch (aiPowerUpType) {
        case 1:
            x2dame_2.setVisible(true);
            x2dame_2.setTouchable(false);
            x2dame_2.invalidate();
            break;
        case 2:
            healthplus_2.setVisible(true);
            healthplus_2.setTouchable(false);
            healthplus_2.invalidate();
            break;
        case 3:
            chakragain_2.setVisible(true);
            chakragain_2.setTouchable(false);
            chakragain_2.invalidate();
            break;
        case 4:
            sheild_2.setVisible(true);
            sheild_2.setTouchable(false);
            sheild_2.invalidate();
            break;
    }
}

void GameScreenView::hideAllPlayerPowerUps()
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

void GameScreenView::hideAllAIPowerUps()
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

void GameScreenView::clearPowerUpAfterUse(bool isPlayer)
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

int GameScreenView::generateRandomNumber(int min, int max)
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

int GameScreenView::calculateDamage(int baseDamage, bool isDefending, bool hasDoubleAttack, bool hasShield)
{
    if (hasShield) {
        return 0;
    }

    int finalDamage = baseDamage;

    if (hasDoubleAttack) {
        finalDamage = (int)(baseDamage * 2.5);
    }

    if (isDefending) {
        if (finalDamage == 40 || (hasDoubleAttack && baseDamage == 40)) {
            finalDamage = 25;
        } else {
            finalDamage = finalDamage / 2;
        }
    }

    return finalDamage;
}

void GameScreenView::playerAttack()
{
    resetNarutoState();
    naruto.setVisible(false);
    naruto_atk.setVisible(true);
    naruto_atk.invalidate();
    naruto.invalidate();

    int baseDamage = generateRandomNumber(3, 8);
    bool hasDoubleAttack = (playerPowerUpActive && playerPowerUpType == 1);
    bool aiHasShield = (aiPowerUpActive && aiPowerUpType == 4);
    int actualDamage = calculateDamage(baseDamage, aiDefending, hasDoubleAttack, aiHasShield);

    aiHP -= actualDamage;
    if (aiHP < 0) aiHP = 0;

    showDamageText(0, actualDamage);

    playerChakraLevel += generateRandomNumber(5, 10);
    if (playerChakraLevel > 100) playerChakraLevel = 100;

    if (hasDoubleAttack) {
        clearPowerUpAfterUse(true);
    }
    if (aiHasShield && actualDamage == 0) {
        clearPowerUpAfterUse(false);
    }

    updateAIHPDisplay();
    updatePlayerChakraDisplay();
    toggleLED();

    playerActionDone = true;
    playerSpecialUsed = false;

    if (aiHP <= 0) {
        startResetTimer();
        return;
    }
    startResetTimer();
}

void GameScreenView::playerDefend()
{
    resetNarutoState();
    naruto.setVisible(false);
    naruto_def.setVisible(true);
    naruto_def.invalidate();
    naruto.invalidate();

    playerDefending = true;

    int healAmount = generateRandomNumber(2, 5);
    playerHP += healAmount;
    if (playerHP > 100) playerHP = 100;

    playerChakraLevel += generateRandomNumber(8, 15);
    if (playerChakraLevel > 100) playerChakraLevel = 100;

    showDamageText(0, 0);

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

void GameScreenView::playerSpecial()
{
    if (gameEnded || playerChakraLevel < 100) return;

    resetNarutoState();
    naruto.setVisible(false);
    naruto_spe.setVisible(true);
    naruto_spe.invalidate();
    naruto.invalidate();

    int baseDamage = 40;
    bool aiHasShield = (aiPowerUpActive && aiPowerUpType == 4);

    int actualDamage = calculateSpecialDamage(baseDamage, aiDefending, aiHasShield);

    aiHP -= actualDamage;
    if (aiHP < 0) aiHP = 0;

    showDamageText(0, actualDamage);

    playerChakraLevel = 0;

    if (aiHasShield && actualDamage == 0) {
        clearPowerUpAfterUse(false);
    }

    updatePlayerChakraDisplay();
    updateAIHPDisplay();

    playerActionDone = true;
    playerSpecialUsed = true;

    if (aiHP <= 0) {
        toggleLED();
        if (!gameEnded) {
            endGame(true);
        }
    }

    toggleLED();
    startResetTimer();
}

void GameScreenView::performAIAction()
{
    if (gameEnded || aiHP <= 0) {
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
        int aiHPPercent = (aiHP * 100) / 100;
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
        }
    }

    switch(action)
    {
        case 1:
            if (!gameEnded && aiHP > 0) aiAttack();
            break;
        case 2:
            if (!gameEnded && aiHP > 0) aiDefend();
            break;
        case 3:
            if (!gameEnded && aiHP > 0 && aiChakraLevel >= 100) aiSpecial();
            break;
    }
}

void GameScreenView::aiAttack()
{
    if (gameEnded) return;

    resetBleachState();

    bleach.setVisible(false);
    bleach_atk.setVisible(true);
    bleach_atk.invalidate();
    bleach.invalidate();

    int baseDamage = generateRandomNumber(3, 8);
    bool hasDoubleAttack = (aiPowerUpActive && aiPowerUpType == 1);
    bool playerHasShield = (playerPowerUpActive && playerPowerUpType == 4);

    int actualDamage = calculateDamage(baseDamage, playerDefending, hasDoubleAttack, playerHasShield);

    playerHP -= actualDamage;
    if (playerHP < 0) playerHP = 0;

    showDamageText(actualDamage, 0);

    aiChakraLevel += generateRandomNumber(5, 10);
    if (aiChakraLevel > 100) aiChakraLevel = 100;

    if (hasDoubleAttack) {
        clearPowerUpAfterUse(false);
        toggleLED();
        toggleLED();
    }

    if (playerHasShield && actualDamage == 0) {
        clearPowerUpAfterUse(true);
    }

    updatePlayerHPDisplay();
    updateAIChakraDisplay();

    aiActionDone = true;
}

void GameScreenView::aiDefend()
{
    if (gameEnded) return;

    resetBleachState();

    bleach.setVisible(false);
    bleach_def.setVisible(true);
    bleach_def.invalidate();
    bleach.invalidate();

    aiDefending = true;

    int healAmount = generateRandomNumber(2, 5);
    aiHP += healAmount;
    if (aiHP > 100) aiHP = 100;

    aiChakraLevel += generateRandomNumber(8, 15);
    if (aiChakraLevel > 100) aiChakraLevel = 100;

    showDamageText(0, 0);

    if (aiPowerUpActive && aiPowerUpType == 1) {
        clearPowerUpAfterUse(false);
    }

    updateAIHPDisplay();
    updateAIChakraDisplay();

    aiActionDone = true;
}

void GameScreenView::aiSpecial()
{
    if (aiChakraLevel < 100 || gameEnded) {
        return;
    }

    resetBleachState();

    bleach.setVisible(false);
    bleach_spe.setVisible(true);
    bleach_spe.invalidate();
    bleach.invalidate();

    int baseDamage = 40;
    bool playerHasShield = (playerPowerUpActive && playerPowerUpType == 4);

    int actualDamage = calculateSpecialDamage(baseDamage, playerDefending, playerHasShield);

    playerHP -= actualDamage;
    if (playerHP < 0) playerHP = 0;

    showDamageText(actualDamage, 0);

    aiChakraLevel = 0;

    if (playerHasShield && actualDamage == 0) {
        clearPowerUpAfterUse(true);
    }

    updateAIChakraDisplay();
    updatePlayerHPDisplay();

    aiActionDone = true;
    aiSpecialUsed = true;
}

void GameScreenView::resetNarutoState()
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

void GameScreenView::resetBleachState()
{
    bleach_atk.setVisible(false);
    bleach_def.setVisible(false);
    bleach_spe.setVisible(false);
    bleach_lose.setVisible(false);
    bleach_win.setVisible(false);

    if (aiHP > 0 && !gameEnded) {
        bleach.setVisible(true);
    }

    bleach_atk.invalidate();
    bleach_def.invalidate();
    bleach_spe.invalidate();
    bleach_lose.invalidate();
    bleach_win.invalidate();
    bleach.invalidate();
}

void GameScreenView::startResetTimer()
{
    resetTimer = 0;
    resetTimerActive = true;
}

void GameScreenView::updatePlayerHPDisplay()
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

void GameScreenView::updateAIHPDisplay()
{
    hideAllAIHPWidgets();

    int roundedHP = (aiHP / 10) * 10;

    if (roundedHP < 0) roundedHP = 0;
    if (roundedHP > 100) roundedHP = 100;

    if (aiHP == 0 || roundedHP == 0) {
        BITMAP_HP_0_ID_1.setVisible(true);
        BITMAP_HP_0_ID_1.invalidate();

        if (!gameEnded) {
            endGame(true);
        }
        return;
    }

    switch (roundedHP) {
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

void GameScreenView::hideAllPlayerHPWidgets()
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

void GameScreenView::hideAllAIHPWidgets()
{
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

int GameScreenView::calculateSpecialDamage(int baseDamage, bool isDefending, bool hasShield)
{
    if (hasShield) {
        return 0;
    }

    int finalDamage = baseDamage;

    if (isDefending) {
        finalDamage = 25;
    }

    return finalDamage;
}

void GameScreenView::updatePlayerChakraDisplay()
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

void GameScreenView::updateAIChakraDisplay()
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

void GameScreenView::gainChakra(int amount)
{
    playerChakraLevel += amount;
    if (playerChakraLevel > 100)
    {
        playerChakraLevel = 100;
    }

    updatePlayerChakraDisplay();
}

void GameScreenView::useChakra(int amount)
{
    playerChakraLevel -= amount;
    if (playerChakraLevel < 0)
    {
        playerChakraLevel = 0;
    }

    updatePlayerChakraDisplay();
}

void GameScreenView::checkGameEnd()
{
    return;
}

void GameScreenView::endGame(bool playerWon)
{
    gameEnded = true;

    playerWonGame = playerWon;

    hideActionButtons();
    btn_arrow1.setVisible(false);
    btn_arrow1.invalidate();

    hideAllPlayerPowerUps();
    hideAllAIPowerUps();
    hideDamageText();

    naruto.setVisible(false);
    naruto_atk.setVisible(false);
    naruto_def.setVisible(false);
    naruto_spe.setVisible(false);

    bleach.setVisible(false);
    bleach_atk.setVisible(false);
    bleach_def.setVisible(false);
    bleach_spe.setVisible(false);

    gameover.setVisible(false);
    win.setVisible(false);

    if (playerWon) {
        for (int i = 0; i < 8; i++) {
            toggleLED();
        }

        naruto_lose.setVisible(false);
        bleach_win.setVisible(false);

        naruto_win.setVisible(true);
        bleach_lose.setVisible(true);
        win.setVisible(true);

        naruto_win.invalidate();
        bleach_lose.invalidate();
        win.invalidate();

        for (int i = 0; i < 6; i++) {
            toggleLED();
        }

        for (int i = 0; i < 3; i++) {
            toggleLED();
        }
    } else {
        naruto_win.setVisible(false);
        bleach_lose.setVisible(false);

        naruto_lose.setVisible(true);
        bleach_win.setVisible(true);
        gameover.setVisible(true);

        naruto_lose.invalidate();
        bleach_win.invalidate();
        gameover.invalidate();

        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);

        for (int i = 0; i < 5; i++) {
            toggleLED();
        }
    }

    autoReturnTimer = 0;
    autoReturnTimerActive = true;

    toggleLED();
    toggleLED();
    toggleLED();

    gameEnded = true;
}
