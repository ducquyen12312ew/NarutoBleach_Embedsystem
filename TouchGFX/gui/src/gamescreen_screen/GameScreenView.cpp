#include <gui/gamescreen_screen/GameScreenView.hpp>
#include <touchgfx/Color.hpp>
#include "stm32f4xx_hal.h"
#include <gui/character_screen/CharacterView.hpp>

GameScreenView::GameScreenView()
{
    resetTimer = 0;
    resetTimerActive = false;
    randomSeed = 12345; // Khởi tạo seed cho random

    // ===== DAMAGE DISPLAY SYSTEM INITIALIZATION - NEW =====
    damageDisplayTimer = 0;
    damageDisplayActive = false;
    currentDamageAmount = 0;
    isPlayerDamage = false;
    damageTextStartY = 99;       // Default Y position for player damage
    damageTextAIStartY = 141;    // Default Y position for AI damage
}


void GameScreenView::setupScreen()
{
    GameScreenViewBase::setupScreen();
    initGPIO();

    // Improved random seed
    randomSeed = HAL_GetTick();
    if (randomSeed == 0) randomSeed = 12345;
    randomSeed = (randomSeed * 1664525 + 1013904223);

    // Initialize game state
    playerHP = 100;
    aiHP = 100;
    playerChakraLevel = 0;
    aiChakraLevel = 0;
    menuOpen = false;
    gameEnded = false;
    playerWonGame = false;

    // Initialize combat state
    playerDefending = false;
    aiDefending = false;
    playerSpecialUsed = false;
    aiSpecialUsed = false;

    // Initialize power-up system
    turnCounter = 0;
    playerPowerUpType = 0;
    aiPowerUpType = 0;
    playerPowerUpActive = false;
    aiPowerUpActive = false;
    playerActionDone = false;
    aiActionDone = false;

    // Hide all damage text when starting game
    hideDamageText();

    // Reset damage display variables
    damageDisplayTimer = 0;
    damageDisplayActive = false;
    currentDamageAmount = 0;
    isPlayerDamage = false;

    // Set starting positions for damage text
    damageTextStartY = 99;
    damageTextAIStartY = 141;

    // Initialize AI personality
    int personalityRoll = generateRandomNumber(1, 100);
    if (personalityRoll <= 70) {
        aiPersonality = 0; // Aggressive
    } else if (personalityRoll <= 90) {
        aiPersonality = 1; // Defensive
    } else {
        aiPersonality = 2; // Random
    }

    // ===== KHỞI TẠO TIMER SYSTEM =====
    resetTimer = 0;
    resetTimerActive = false;
    autoReturnTimer = 0;
    autoReturnTimerActive = false;

    // Hide all power-up icons when starting
    hideAllPlayerPowerUps();
    hideAllAIPowerUps();

    // Update initial UI
    updatePlayerHPDisplay();
    updateAIHPDisplay();
    updatePlayerChakraDisplay();
    updateAIChakraDisplay();

    // Ensure both characters are in normal state when starting
    resetNarutoState();
    resetBleachState();
}

void GameScreenView::showDamageText(int playerDamage, int aiDamage)
{
    // Ẩn tất cả damage text trước
    hideDamageText();

    // ===== SETUP TRACKING VARIABLES =====
    currentPlayerDamage = playerDamage;
    currentAIDamage = aiDamage;
    playerDamageActive = (playerDamage > 0);
    aiDamageActive = (aiDamage > 0);

    // ===== HIỂN THỊ DAMAGE CHO PLAYER (NARUTO) =====
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
        // DEBUG: LED 1 lần cho player damage
        toggleLED();
    }

    // ===== HIỂN THỊ DAMAGE CHO AI (BLEACH) - QUAN TRỌNG! =====
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
        // DEBUG: LED 2 lần cho AI damage
        toggleLED();
        toggleLED();
    }

    // Bật damage display timer nếu có damage
    if (playerDamage > 0 || aiDamage > 0) {
        damageDisplayTimer = 0;
        damageDisplayActive = true;

        // Extra LED nếu cả 2 bên đều có damage
        if (playerDamage > 0 && aiDamage > 0) {
            toggleLED();
            toggleLED();
            toggleLED();
        }
    }
}

void GameScreenView::hideDamageText()
{
    // ẨN PLAYER DAMAGE
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

    // ẨN AI DAMAGE
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

    // INVALIDATE ALL
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

    // ===== RESET FLAGS =====
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

    damageDisplayTimer += 16; // TouchGFX tick ~16ms
    float progress = (float)damageDisplayTimer / DAMAGE_DISPLAY_TIME_MS;
    if (progress > 1.0f) progress = 1.0f;

    // FADE OUT EFFECT
    int alpha = 255 - (int)(progress * 255);
    if (alpha < 0) alpha = 0;

    // ===== CẬP NHẬT ALPHA CHO PLAYER DAMAGE =====
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

    // ===== CẬP NHẬT ALPHA CHO AI DAMAGE - QUAN TRỌNG! =====
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

    // Ẩn khi hết thời gian
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
        // ===== EASTER EGG: CLICK BLEACH = INSTANT WIN =====
        if (!gameEnded) {
            // Detect click on Bleach character area
            // Bleach character thường ở bên phải, khoảng x: 160-240, y: 140-280
            bool clickedOnBleach = (evt.getX() >= 160 && evt.getX() <= 240 &&
                                   evt.getY() >= 140 && evt.getY() <= 280);

            // Hoặc detect click trên bất kỳ Bleach sprite nào đang hiển thị
            bool clickedOnBleachSprite = false;

            // Check if click intersects với bất kỳ Bleach widget nào
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
                // ===== EASTER EGG ACTIVATED! =====

                // Special LED pattern để báo hiệu easter egg
                for (int i = 0; i < 20; i++) {
                    toggleLED();
                }

                // Instant kill AI với style!
                aiHP = 0;

                // Show dramatic damage effect
                showDamageText(0, 999); // Show massive damage to AI

                // Force reset cả 2 character về normal trước khi end
                resetNarutoState();
                resetBleachState();

                // Update HP display immediately
                updateAIHPDisplay();

                // Extra celebration LEDs
                for (int i = 0; i < 25; i++) {
                    toggleLED();
                }

                // Hide action menu
                hideActionButtons();

                // Note: endGame(true) sẽ được gọi tự động trong updateAIHPDisplay()
                // khi nó detect aiHP = 0

                return; // Exit sớm để không xử lý click khác
            }
        }

        // ===== DEBUG CLICKS (GIỮ NGUYÊN) =====
        // DEBUG: Click ở góc trên trái để force kill AI
        if (evt.getX() < 50 && evt.getY() < 50) {
            aiHP = 0;
            updateAIHPDisplay();
            return;
        }

        // DEBUG: Click ở góc trên phải để force kill PLAYER
        if (evt.getX() > 190 && evt.getY() < 50) {
            playerHP = 0;
            updatePlayerHPDisplay();
            return;
        }

        // [TIẾP TỤC VỚI PHẦN CODE CLICK XỬ LÝ GAME LOGIC BÌNH THƯỜNG...]

        // DEBUG: Click ở góc dưới trái để kiểm tra game state
        if (evt.getX() < 50 && evt.getY() > 270) {
            debugGameState();
            return;
        }

        // DEBUG: Click ở góc dưới phải để GUARANTEED power-ups
        if (evt.getX() > 190 && evt.getY() > 270 && !gameEnded) {
            giveRandomPowerUps();
            return;
        }

        // DEBUG: Click ở giữa dưới để xem turn counter + force complete turn
        if (evt.getX() > 100 && evt.getX() < 140 && evt.getY() > 270 && !gameEnded) {
            for (int i = 0; i < turnCounter + 1; i++) {
                toggleLED();
            }
            playerActionDone = true;
            aiActionDone = true;
            checkTurnComplete();
            return;
        }

        // ===== NORMAL GAME LOGIC (GIỮ NGUYÊN) =====
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
    // Only process timers when game hasn't ended
    if (!gameEnded) {
        // Handle auto reset timer
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

        // Handle damage display timer
        updateDamageDisplay();
    }
    else {
        // ===== XỬ LÝ AUTO RETURN TIMER KHI GAME ĐÃ KẾT THÚC =====
        if (autoReturnTimerActive) {
            autoReturnTimer += 16; // TouchGFX tick ~16ms

            if (autoReturnTimer >= AUTO_RETURN_TIME_MS) {
                // Tắt LED trước khi chuyển screen
                HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);

                // ===== ROUTING THEO KẾT QUẢ GAME =====
                if (playerWonGame) {
                    // Player thắng -> BackUpScreen (Character unlock)
                    application().gotoBackUpScreenScreenNoTransition();
                } else {
                    // Player thua -> MainMenuScreen
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
    // FORCE DEBUG: LED báo hiệu đang give power-ups
    for (int i = 0; i < 8; i++) {
        toggleLED();
    }

    // ===== EXTREME HEAL/CHAKRA FOCUS: Heal 40%, Chakra 35%, X2Dame 15%, Shield 10% =====

    // PLAYER POWER-UP
    int playerRoll = generateRandomNumber(1, 100);
    int newPlayerPowerUp;

    if (playerRoll >= 1 && playerRoll <= 40) {
        newPlayerPowerUp = 2; // 40% - Heal (VERY HIGH)
    } else if (playerRoll >= 41 && playerRoll <= 75) {
        newPlayerPowerUp = 3; // 35% - Chakra (HIGH)
    } else if (playerRoll >= 76 && playerRoll <= 90) {
        newPlayerPowerUp = 1; // 15% - Double Damage (VERY LOW)
    } else { // 91-100
        newPlayerPowerUp = 4; // 10% - Shield (LOW)
    }

    playerPowerUpType = newPlayerPowerUp;
    playerPowerUpActive = true;
    applyInstantPowerUp(true, playerPowerUpType);
    updatePlayerPowerUpDisplay();

    // AI POWER-UP with SAME DISTRIBUTION
    int aiRoll = generateRandomNumber(1, 100);
    int newAIPowerUp;

    if (aiRoll >= 1 && aiRoll <= 40) {
          newAIPowerUp = 2; // 40% - Heal (TĂNG từ 30%)
      } else if (aiRoll >= 41 && aiRoll <= 70) {
          newAIPowerUp = 3; // 30% - Chakra (TĂNG từ 25%)
      } else if (aiRoll >= 71 && aiRoll <= 85) {
          newAIPowerUp = 1; // 15% - Double Damage (GIẢM từ 22%)
      } else { // 86-100
          newAIPowerUp = 4; // 15% - Shield (GIẢM từ 23%)
      }

    aiPowerUpType = newAIPowerUp;
    aiPowerUpActive = true;
    applyInstantPowerUp(false, aiPowerUpType);
    updateAIPowerUpDisplay();

    // Debug patterns
    for (int i = 0; i < playerPowerUpType; i++) {
        toggleLED();
    }

    for (int pause = 0; pause < 1000; pause++) {
        // Delay
    }

    for (int i = 0; i < aiPowerUpType + 5; i++) {
        toggleLED();
    }
}

// Debug function
void GameScreenView::debugGameState()
{
    if (gameEnded) {
        // LED báo hiệu đang debug - game đã kết thúc
        for (int i = 0; i < 10; i++) {
            toggleLED();
        }
    } else {
        // LED pattern để báo power-up status:
        // - 1 blink: No power-ups
        // - 2 blinks: Player có power-up
        // - 3 blinks: AI có power-up
        // - 4 blinks: Cả 2 đều có power-up

        int blinkCount = 1; // Default: no power-ups

        if (playerPowerUpActive && aiPowerUpActive) {
            blinkCount = 4; // Cả 2 có
        } else if (playerPowerUpActive) {
            blinkCount = 2; // Chỉ player có
        } else if (aiPowerUpActive) {
            blinkCount = 3; // Chỉ AI có
        }

        for (int i = 0; i < blinkCount; i++) {
            toggleLED();
            // Small delay between blinks (có thể implement nếu cần)
        }
    }
}



// ==================== POWER-UP SYSTEM ====================


void GameScreenView::checkTurnComplete()
{
    // CHỈ KIỂM TRA KHI CẢ 2 ĐÃ ACTION
    if (playerActionDone && aiActionDone) {
        turnCounter++;

        // LED báo hiệu hoàn thành turn
        toggleLED();

        // ===== FIXED: CHỈ 20% CHANCE POWER-UP VÀ CHỈ KHI KHÔNG DÙNG SPECIAL =====
        if (!playerSpecialUsed && !aiSpecialUsed) {
            // GIẢM TỪ 70% XUỐNG 20%
            int powerUpChance = generateRandomNumber(1, 100);
            if (powerUpChance <= 20) {
                giveRandomPowerUps();
            }
        }
        // NẾU CÓ AI DÙNG SPECIAL → KHÔNG CÓ POWER-UP

        // Clear non-persistent power-ups
        if (playerPowerUpActive && (playerPowerUpType == 2 || playerPowerUpType == 3 || playerPowerUpType == 4)) {
            clearPowerUpAfterUse(true);
        }
        if (aiPowerUpActive && (aiPowerUpType == 2 || aiPowerUpType == 3 || aiPowerUpType == 4)) {
            clearPowerUpAfterUse(false);
        }

        // Reset
        playerActionDone = false;
        aiActionDone = false;
        playerSpecialUsed = false;
        aiSpecialUsed = false;
    }
}


void GameScreenView::giveRandomPowerUps()
{
    // FORCE DEBUG: LED báo hiệu đang give power-ups
    for (int i = 0; i < 3; i++) { // GIẢM LED debug
        toggleLED();
    }

    // ===== NEW BALANCED DISTRIBUTION =====
    // Shield 35%, X2Dame 30%, Healthplus 20%, Chakragain 15%

    // PLAYER POWER-UP
    int playerRoll = generateRandomNumber(1, 100);
    int newPlayerPowerUp;

    if (playerRoll >= 1 && playerRoll <= 35) {
        newPlayerPowerUp = 4; // 35% - Shield (HIGHEST)
    } else if (playerRoll >= 36 && playerRoll <= 65) {
        newPlayerPowerUp = 1; // 30% - Double Damage
    } else if (playerRoll >= 66 && playerRoll <= 85) {
        newPlayerPowerUp = 2; // 20% - Heal
    } else { // 86-100
        newPlayerPowerUp = 3; // 15% - Chakra (LOWEST)
    }

    playerPowerUpType = newPlayerPowerUp;
    playerPowerUpActive = true;
    applyInstantPowerUp(true, playerPowerUpType);
    updatePlayerPowerUpDisplay();

    // AI POWER-UP - SAME DISTRIBUTION
    int aiRoll = generateRandomNumber(1, 100);
    int newAIPowerUp;

    if (aiRoll >= 1 && aiRoll <= 35) {
        newAIPowerUp = 4; // 35% - Shield
    } else if (aiRoll >= 36 && aiRoll <= 65) {
        newAIPowerUp = 1; // 30% - Double Damage
    } else if (aiRoll >= 66 && aiRoll <= 85) {
        newAIPowerUp = 2; // 20% - Heal
    } else {
        newAIPowerUp = 3; // 15% - Chakra
    }

    aiPowerUpType = newAIPowerUp;
    aiPowerUpActive = true;
    applyInstantPowerUp(false, aiPowerUpType);
    updateAIPowerUpDisplay();
}

void GameScreenView::giveRandomPowerUpsAlternative()
{
    // FORCED ROTATION to ensure all power-ups appear equally
    static int playerRotation = 1;
    static int aiRotation = 1;

    // FORCE DEBUG: LED báo hiệu đang give power-ups
    for (int i = 0; i < 8; i++) {
        toggleLED();
    }

    // Player gets power-up in rotation order
    playerPowerUpType = playerRotation;
    playerPowerUpActive = true;
    applyInstantPowerUp(true, playerPowerUpType);
    updatePlayerPowerUpDisplay();

    // Advance player rotation
    playerRotation++;
    if (playerRotation > 4) playerRotation = 1;

    // AI gets power-up in rotation order (independent)
    aiPowerUpType = aiRotation;
    aiPowerUpActive = true;
    applyInstantPowerUp(false, aiPowerUpType);
    updateAIPowerUpDisplay();

    // Advance AI rotation
    aiRotation++;
    if (aiRotation > 4) aiRotation = 1;

    // Debug LED patterns
    for (int i = 0; i < playerPowerUpType; i++) {
        toggleLED();
    }

    for (int pause = 0; pause < 1000; pause++) {
        // Delay
    }

    for (int i = 0; i < aiPowerUpType + 5; i++) {
        toggleLED();
    }
}

void GameScreenView::applyInstantPowerUp(bool isPlayer, int type)
{
    switch (type) {
        case 1: // Double Damage - chờ đến khi combat
            // Không làm gì ngay, sẽ apply trong calculateDamage()
            break;

        case 2: // Heal - GIẢM THÊM
            if (isPlayer) {
                playerHP += 5; // GIẢM TỪ 10 XUỐNG 5
                if (playerHP > 100) playerHP = 100;
                updatePlayerHPDisplay();
            } else {
                aiHP += 5; // GIẢM TỪ 10 XUỐNG 5
                if (aiHP > 100) aiHP = 100;
                updateAIHPDisplay();
            }
            break;

        case 3: // Chakra - GIẢM THÊM
            if (isPlayer) {
                playerChakraLevel += 15; // GIẢM TỪ 20 XUỐNG 15
                if (playerChakraLevel > 100) playerChakraLevel = 100;
                updatePlayerChakraDisplay();
            } else {
                aiChakraLevel += 15; // GIẢM TỪ 20 XUỐNG 15
                if (aiChakraLevel > 100) aiChakraLevel = 100;
                updateAIChakraDisplay();
            }
            break;

        case 4: // Shield
            // Không làm gì ngay, sẽ apply trong calculateDamage()
            break;
    }
}

void GameScreenView::updatePlayerPowerUpDisplay()
{
    // Ẩn tất cả trước
    hideAllPlayerPowerUps();

    if (!playerPowerUpActive) return;

    // FORCE SHOW với enhanced refresh
    switch (playerPowerUpType) {
        case 1: // Double Damage
            x2dame_1.setVisible(true);
            x2dame_1.invalidate();
            break;

        case 2: // Heal - FORCE SHOW
            healthplus_1.setVisible(true);
            healthplus_1.setAlpha(255);
            healthplus_1.setPosition(9, 61, 32, 37);
            healthplus_1.invalidate();
            // Force double refresh
            healthplus_1.setVisible(false);
            healthplus_1.setVisible(true);
            healthplus_1.invalidate();
            break;

        case 3: // Chakra - FORCE SHOW
            chakragain_1.setVisible(true);
            chakragain_1.setAlpha(255);
            chakragain_1.setPosition(9, 61, 32, 37);
            chakragain_1.invalidate();
            // Force double refresh
            chakragain_1.setVisible(false);
            chakragain_1.setVisible(true);
            chakragain_1.invalidate();
            break;

        case 4: // Shield
            sheild_1.setVisible(true);
            sheild_1.setAlpha(255);
            sheild_1.setPosition(8, 59, 32, 37);
            sheild_1.invalidate();
            break;
    }

    // Force screen refresh
    invalidate();
}

void GameScreenView::updateAIPowerUpDisplay()
{
    // Ẩn tất cả power-up icons của AI trước
    hideAllAIPowerUps();

    // CHỈ HIỂN THỊ KHI CÓ POWER-UP ACTIVE
    if (!aiPowerUpActive) {
        return;
    }

    // Hiển thị icon tương ứng với power-up type
    switch (aiPowerUpType) {
        case 1: // Double Damage - Persistent cho đến khi dùng
            x2dame_2.setVisible(true);
            x2dame_2.setTouchable(false);
            x2dame_2.invalidate();
            break;
        case 2: // Heal - HIỂN THỊ ICON
            healthplus_2.setVisible(true);
            healthplus_2.setTouchable(false);
            healthplus_2.invalidate();
            break;
        case 3: // Chakra - HIỂN THỊ ICON
            chakragain_2.setVisible(true);
            chakragain_2.setTouchable(false);
            chakragain_2.invalidate();
            break;
        case 4: // Shield - Hiển thị 1 turn rồi clear
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

// ==================== UTILITY FUNCTIONS ====================

int GameScreenView::generateRandomNumber(int min, int max)
{
    // IMPROVED LINEAR CONGRUENTIAL GENERATOR
    randomSeed = (randomSeed * 1664525 + 1013904223) & 0x7fffffff;

    // Additional mixing to improve distribution
    uint32_t mixed = randomSeed;
    mixed ^= mixed >> 16;
    mixed *= 0x85ebca6b;
    mixed ^= mixed >> 13;
    mixed *= 0xc2b2ae35;
    mixed ^= mixed >> 16;

    return min + (mixed % (max - min + 1));
}

// Cập nhật calculateDamage function để hỗ trợ power-ups
int GameScreenView::calculateDamage(int baseDamage, bool isDefending, bool hasDoubleAttack, bool hasShield)
{
    // Shield blocks everything completely
    if (hasShield) {
        return 0; // Shield chặn hoàn toàn
        // NOTE: Shield sẽ bị clear tự động trong checkTurnComplete()
        // Không cần clear ở đây nữa
    }

    int finalDamage = baseDamage;

    // Apply double damage first
    if (hasDoubleAttack) {
        finalDamage = (int)(baseDamage * 2.5); // x2.5 như yêu cầu
    }

    // Apply defense reduction
    if (isDefending) {
        if (finalDamage == 40 || (hasDoubleAttack && baseDamage == 40)) {
            // Special attack khi bị defend: giảm xuống 25 damage
            finalDamage = 25;
        } else {
            // Đòn thường khi bị defend: giảm 50%
            finalDamage = finalDamage / 2;
        }
    }

    return finalDamage;
}

// ==================== PLAYER COMBAT FUNCTIONS ====================

void GameScreenView::playerAttack()
{
    // Reset và animation
    resetNarutoState();
    naruto.setVisible(false);
    naruto_atk.setVisible(true);
    naruto_atk.invalidate();
    naruto.invalidate();

    // Damage calculation
    int baseDamage = generateRandomNumber(3, 8);
    bool hasDoubleAttack = (playerPowerUpActive && playerPowerUpType == 1);
    bool aiHasShield = (aiPowerUpActive && aiPowerUpType == 4);
    int actualDamage = calculateDamage(baseDamage, aiDefending, hasDoubleAttack, aiHasShield);

    // Apply damage
    aiHP -= actualDamage;
    if (aiHP < 0) aiHP = 0;

    // ===== HIỆN DAMAGE CẢ 2 BÊN: Player = 0, AI = actualDamage =====
    showDamageText(0, actualDamage);

    // Gain chakra
    playerChakraLevel += generateRandomNumber(5, 10);
    if (playerChakraLevel > 100) playerChakraLevel = 100;

    // Clear power-ups
    if (hasDoubleAttack) {
        clearPowerUpAfterUse(true);
    }
    if (aiHasShield && actualDamage == 0) {
        clearPowerUpAfterUse(false);
    }

    // Update UI
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
    // Reset và animation
    resetNarutoState();
    naruto.setVisible(false);
    naruto_def.setVisible(true);
    naruto_def.invalidate();
    naruto.invalidate();

    // Set defend state
    playerDefending = true;

    // Heal and chakra gain
    int healAmount = generateRandomNumber(2, 5);
    playerHP += healAmount;
    if (playerHP > 100) playerHP = 100;

    playerChakraLevel += generateRandomNumber(8, 15);
    if (playerChakraLevel > 100) playerChakraLevel = 100;

    // ===== HIỆN HEAL AMOUNT NHƯ NEGATIVE DAMAGE =====
    // Có thể hiển thị heal như +2, +3, +4, +5 thay vì damage
    // Tạm thời không hiện gì vì defend không có damage
    showDamageText(0, 0);

    // Clear double damage if defending
    if (playerPowerUpActive && playerPowerUpType == 1) {
        clearPowerUpAfterUse(true);
    }

    // Update UI
    updatePlayerHPDisplay();
    updatePlayerChakraDisplay();
    toggleLED();

    playerActionDone = true;
    playerSpecialUsed = false;

    startResetTimer();
}

void GameScreenView::playerSpecial()
{
    // Check conditions
    if (gameEnded || playerChakraLevel < 100) return;

    // Reset và animation
    resetNarutoState();
    naruto.setVisible(false);
    naruto_spe.setVisible(true);
    naruto_spe.invalidate();
    naruto.invalidate();

    // ===== SPECIAL DAMAGE FIXED - KHÔNG SỬ DỤNG POWER-UP =====
    int baseDamage = 40;
    // BỎ DOUBLE ATTACK CHO SPECIAL
    // bool hasDoubleAttack = (playerPowerUpActive && playerPowerUpType == 1);
    bool aiHasShield = (aiPowerUpActive && aiPowerUpType == 4);

    // SPECIAL SKILL LUÔN LÀM 40 DAMAGE, CHỈ BỊ GIẢM BỞI DEFEND VÀ SHIELD
    int actualDamage = calculateSpecialDamage(baseDamage, aiDefending, aiHasShield);

    // Apply damage
    aiHP -= actualDamage;
    if (aiHP < 0) aiHP = 0;

    // HIỆN DAMAGE
    showDamageText(0, actualDamage);

    // Reset chakra
    playerChakraLevel = 0;

    // ===== KHÔNG CLEAR PLAYER POWER-UP KHI DÙNG SPECIAL =====
    // BỎ: if (hasDoubleAttack) clearPowerUpAfterUse(true);

    // CHỈ CLEAR AI SHIELD NẾU BLOCK DAMAGE
    if (aiHasShield && actualDamage == 0) {
        clearPowerUpAfterUse(false);
    }

    // Update UI
    updatePlayerChakraDisplay();
    updateAIHPDisplay();

    // ===== SPECIAL = TRUE (NO POWER-UP GENERATION) =====
    playerActionDone = true;
    playerSpecialUsed = true; // KHÔNG có power-up sau special

    if (aiHP <= 0) {
        toggleLED();
        if (!gameEnded) {
            endGame(true);
        }
    }

    toggleLED();
    startResetTimer();
}

// ==================== AI COMBAT FUNCTIONS ====================

void GameScreenView::performAIAction()
{
    // KIỂM TRA QUAN TRỌNG: Nếu game đã kết thúc hoặc AI đã chết thì KHÔNG làm gì
    if (gameEnded || aiHP <= 0) {
        return;
    }

    if (playerHP <= 0) {
        return;
    }

    int action = 1; // Default: attack

    // POWER-UP STRATEGY - SỬA LOGIC
    if (aiPowerUpActive) {
        switch(aiPowerUpType) {
            case 1: // Double Damage Power-Up - ƯU TIÊN ATTACK THAY VÌ SPECIAL
                if (aiChakraLevel >= 100) {
                    int roll = generateRandomNumber(1, 100);
                    if (roll <= 80) {
                        action = 1; // 80% Attack để tận dụng double damage
                    } else if (roll <= 90) {
                        action = 3; // 10% Special
                    } else {
                        action = 2; // 10% Defend
                    }
                } else {
                    // Không đủ chakra cho special: 90% attack, 10% defend
                    if (generateRandomNumber(1, 100) <= 90) {
                        action = 1; // Attack
                    } else {
                        action = 2; // Defend
                    }
                }
                break;

            case 4: // Shield Power-Up - VẪN AGGRESSIVE
                if (aiChakraLevel >= 100) {
                    // 50% special, 50% attack khi có shield
                    if (generateRandomNumber(1, 100) <= 50) {
                        action = 3; // Special
                    } else {
                        action = 1; // Attack
                    }
                } else {
                    // Không đủ chakra: 100% attack
                    action = 1; // Attack
                }
                break;

            case 2: // Heal Power-Up (đã dùng ngay)
            case 3: // Chakra Power-Up (đã dùng ngay)
            default:
                // Dùng AI logic bình thường
                break;
        }
    }

    // NẾU KHÔNG CÓ POWER-UP ĐẶC BIỆT, DÙNG AI LOGIC VỚI PLAYER CHAKRA AWARENESS
    if (!aiPowerUpActive || (aiPowerUpType != 1 && aiPowerUpType != 4)) {
        // Tính phần trăm máu để đưa ra quyết định thông minh
        int aiHPPercent = (aiHP * 100) / 100;
        int playerHPPercent = (playerHP * 100) / 100;

        // CRITICAL: Check if player has 100 chakra (can use special)
        bool playerCanSpecial = (playerChakraLevel >= 100);

        // BASE DEFEND CHANCE increase when player can special
        int baseDefendBonus = 0;
        if (playerCanSpecial) {
            baseDefendBonus = 25; // +25% defend chance khi player có thể dùng special
        }

        // AI Decision making dựa trên personality, tình huống và lượng máu
        switch(aiPersonality)
        {
            case 0: // Aggressive AI (70%) - Nhưng thông minh hơn
                if (aiChakraLevel >= 100) {
                    // Khi có special: điều chỉnh theo player chakra
                    int specialChance = 60;
                    int defendChance = 40 + baseDefendBonus; // Tăng defend khi player có special

                    // Điều chỉnh tỷ lệ dựa trên máu
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

                    // Ensure percentages don't exceed 100
                    if (defendChance > 70) defendChance = 70;
                    if (specialChance < 30) specialChance = 30;

                    int roll = generateRandomNumber(1, 100);
                    if (roll <= defendChance) {
                        action = 2; // Defend
                    } else {
                        action = 3; // Special
                    }
                } else {
                    // Không có special: Quyết định giữa attack và defend
                    if (aiHPPercent >= 70) {
                        int defendChance = 30 + baseDefendBonus;
                        if (defendChance > 70) defendChance = 70;
                        if (generateRandomNumber(1, 100) <= defendChance) {
                            action = 2; // Defend
                        } else {
                            action = 1; // Attack
                        }
                    } else if (aiHPPercent >= 40) {
                        int defendChance = 50 + baseDefendBonus;
                        if (defendChance > 80) defendChance = 80;
                        if (generateRandomNumber(1, 100) <= defendChance) {
                            action = 2; // Defend
                        } else {
                            action = 1; // Attack
                        }
                    } else {
                        int defendChance = 30 + baseDefendBonus;
                        if (defendChance > 60) defendChance = 60;
                        if (generateRandomNumber(1, 100) <= defendChance) {
                            action = 2; // Defend
                        } else {
                            action = 1; // Attack
                        }
                    }
                }
                break;

            case 1: // Defensive AI (20%) - Rất thận trọng + extra defensive với player special
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

                    // Ensure percentages are reasonable
                    if (defendChance > 85) defendChance = 85;
                    if (specialChance < 15) specialChance = 15;

                    int roll = generateRandomNumber(1, 100);
                    if (roll <= defendChance) {
                        action = 2; // Defend
                    } else {
                        action = 3; // Special
                    }
                } else {
                    if (aiHPPercent >= 60) {
                        int defendChance = 80 + baseDefendBonus;
                        if (defendChance > 95) defendChance = 95;
                        if (generateRandomNumber(1, 100) <= defendChance) {
                            action = 2; // Defend
                        } else {
                            action = 1; // Attack
                        }
                    } else if (aiHPPercent >= 30) {
                        int defendChance = 65 + baseDefendBonus;
                        if (defendChance > 90) defendChance = 90;
                        if (generateRandomNumber(1, 100) <= defendChance) {
                            action = 2; // Defend
                        } else {
                            action = 1; // Attack
                        }
                    } else {
                        int defendChance = 40 + baseDefendBonus;
                        if (defendChance > 70) defendChance = 70;
                        if (generateRandomNumber(1, 100) <= defendChance) {
                            action = 2; // Defend
                        } else {
                            action = 1; // Attack
                        }
                    }
                }
                break;

            case 2: // Random AI (10%) - Vẫn có một chút logic về player special
                if (aiChakraLevel >= 100) {
                    int defendChance = 45 + (baseDefendBonus / 2); // Ít sensitive hơn với player special
                    int roll = generateRandomNumber(1, 100);
                    if (roll <= defendChance) {
                        action = 2; // Defend
                    } else {
                        action = 3; // Special
                    }
                } else {
                    if (aiHPPercent >= 70) {
                        int defendChance = 60 + (baseDefendBonus / 2);
                        if (generateRandomNumber(1, 100) <= defendChance) {
                            action = 2; // Defend
                        } else {
                            action = 1; // Attack
                        }
                    } else {
                        // Mostly random but slight defend bias if player can special
                        if (playerCanSpecial && generateRandomNumber(1, 100) <= 30) {
                            action = 2; // Defend
                        } else {
                            action = generateRandomNumber(1, 2);
                        }
                    }
                }
                break;
        }
    }

    // Thực hiện hành động - VỚI KIỂM TRA AN TOÀN
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

    // Reset về trạng thái bình thường trước
    resetBleachState();

    // Hiển thị animation tấn công
    bleach.setVisible(false);
    bleach_atk.setVisible(true);
    bleach_atk.invalidate();
    bleach.invalidate();

    // Tính damage với power-up support
    int baseDamage = generateRandomNumber(3, 8);
    bool hasDoubleAttack = (aiPowerUpActive && aiPowerUpType == 1);
    bool playerHasShield = (playerPowerUpActive && playerPowerUpType == 4);

    int actualDamage = calculateDamage(baseDamage, playerDefending, hasDoubleAttack, playerHasShield);

    // Gây damage lên player
    playerHP -= actualDamage;
    if (playerHP < 0) playerHP = 0;

    // ===== HIỆN DAMAGE CẢ 2 BÊN: Player = actualDamage, AI = 0 =====
    showDamageText(actualDamage, 0);

    // AI gain chakra sau attack
    aiChakraLevel += generateRandomNumber(5, 10);
    if (aiChakraLevel > 100) aiChakraLevel = 100;

    // Clear power-ups
    if (hasDoubleAttack) {
        clearPowerUpAfterUse(false);
        toggleLED();
        toggleLED();
    }

    if (playerHasShield && actualDamage == 0) {
        clearPowerUpAfterUse(true);
    }

    // Cập nhật UI
    updatePlayerHPDisplay();
    updateAIChakraDisplay();

    aiActionDone = true;
}

void GameScreenView::aiDefend()
{
    if (gameEnded) return;

    // Reset về trạng thái bình thường trước
    resetBleachState();

    // Hiển thị animation phòng thủ
    bleach.setVisible(false);
    bleach_def.setVisible(true);
    bleach_def.invalidate();
    bleach.invalidate();

    // Set trạng thái defend
    aiDefending = true;

    // Hồi máu ngẫu nhiên (2-5)
    int healAmount = generateRandomNumber(2, 5);
    aiHP += healAmount;
    if (aiHP > 100) aiHP = 100;

    // Gain chakra (8-15 chakra)
    aiChakraLevel += generateRandomNumber(8, 15);
    if (aiChakraLevel > 100) aiChakraLevel = 100;

    // ===== DEFEND KHÔNG HIỆN DAMAGE =====
    showDamageText(0, 0);

    // Clear double damage power-up nếu có
    if (aiPowerUpActive && aiPowerUpType == 1) {
        clearPowerUpAfterUse(false);
    }

    // Cập nhật UI
    updateAIHPDisplay();
    updateAIChakraDisplay();

    aiActionDone = true;
}

void GameScreenView::aiSpecial()
{
    if (aiChakraLevel < 100 || gameEnded) {
        return;
    }

    // Reset về trạng thái bình thường trước
    resetBleachState();

    // Hiển thị animation special
    bleach.setVisible(false);
    bleach_spe.setVisible(true);
    bleach_spe.invalidate();
    bleach.invalidate();

    // ===== AI SPECIAL DAMAGE FIXED - KHÔNG SỬ DỤNG POWER-UP =====
    int baseDamage = 40;
    // BỎ DOUBLE ATTACK CHO SPECIAL
    // bool hasDoubleAttack = (aiPowerUpActive && aiPowerUpType == 1);
    bool playerHasShield = (playerPowerUpActive && playerPowerUpType == 4);

    // AI SPECIAL CŨNG LÀM 40 DAMAGE, CHỈ BỊ GIẢM BỞI DEFEND VÀ SHIELD
    int actualDamage = calculateSpecialDamage(baseDamage, playerDefending, playerHasShield);

    // Gây damage lên player
    playerHP -= actualDamage;
    if (playerHP < 0) playerHP = 0;

    // HIỆN DAMAGE
    showDamageText(actualDamage, 0);

    // RESET chakra về 0
    aiChakraLevel = 0;

    // ===== KHÔNG CLEAR AI POWER-UP KHI DÙNG SPECIAL =====
    // BỎ: if (hasDoubleAttack) clearPowerUpAfterUse(false);

    // CHỈ CLEAR PLAYER SHIELD NẾU BLOCK DAMAGE
    if (playerHasShield && actualDamage == 0) {
        clearPowerUpAfterUse(true);
    }

    // Cập nhật UI
    updateAIChakraDisplay();
    updatePlayerHPDisplay();

    // ===== AI SPECIAL = TRUE (NO POWER-UP GENERATION) =====
    aiActionDone = true;
    aiSpecialUsed = true; // KHÔNG có power-up sau special
}

// ==================== CHARACTER STATE FUNCTIONS ====================

void GameScreenView::resetNarutoState()
{
    // Ẩn tất cả các trạng thái đặc biệt của naruto
    naruto_atk.setVisible(false);
    naruto_def.setVisible(false);
    naruto_spe.setVisible(false);
    naruto_lose.setVisible(false);
    naruto_win.setVisible(false);

    // Hiển thị lại naruto bình thường (chỉ khi chưa thua và game chưa kết thúc)
    if (playerHP > 0 && !gameEnded) {
        naruto.setVisible(true);
    }

    // Invalidate tất cả
    naruto_atk.invalidate();
    naruto_def.invalidate();
    naruto_spe.invalidate();
    naruto_lose.invalidate();
    naruto_win.invalidate();
    naruto.invalidate();
}

void GameScreenView::resetBleachState()
{
    // Ẩn tất cả các trạng thái đặc biệt của bleach
    bleach_atk.setVisible(false);
    bleach_def.setVisible(false);
    bleach_spe.setVisible(false);
    bleach_lose.setVisible(false);
    bleach_win.setVisible(false);

    // Hiển thị lại bleach bình thường (chỉ khi chưa thua và game chưa kết thúc)
    if (aiHP > 0 && !gameEnded) {
        bleach.setVisible(true);
    }

    // Invalidate tất cả
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

// ==================== HP DISPLAY SYSTEM ====================

void GameScreenView::updatePlayerHPDisplay()
{
    // Ẩn tất cả HP widgets của player trước
    hideAllPlayerHPWidgets();

    // Làm tròn xuống bội số 10
    int roundedHP = (playerHP / 10) * 10;

    // Đảm bảo roundedHP trong phạm vi hợp lệ
    if (roundedHP < 0) roundedHP = 0;
    if (roundedHP > 100) roundedHP = 100;

    // DEBUG: Force test khi Player HP = 0
    if (playerHP == 0 || roundedHP == 0) {
        // FORCE HIỂN THỊ HP_0_ID
        BITMAP_HP_0_ID.setVisible(true);
        BITMAP_HP_0_ID.invalidate();

        // DEBUG: LED blink nhiều lần
        for (int i = 0; i < 10; i++) {
            toggleLED();
        }

        // FORCE END GAME
        if (!gameEnded) {
            endGame(false); // Player thua
        }
        return;
    }

    // Hiển thị widget HP phù hợp cho player
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
            // Fallback case - cũng force về 0
            BITMAP_HP_0_ID.setVisible(true);
            BITMAP_HP_0_ID.invalidate();
            if (!gameEnded) {
                endGame(false); // Player thua
            }
            break;
    }
}

void GameScreenView::updateAIHPDisplay()
{
    // Ẩn tất cả HP widgets của AI trước
    hideAllAIHPWidgets();

    // Làm tròn xuống bội số 10
    int roundedHP = (aiHP / 10) * 10;

    // Đảm bảo roundedHP trong phạm vi hợp lệ
    if (roundedHP < 0) roundedHP = 0;
    if (roundedHP > 100) roundedHP = 100;

    // Kiểm tra game end khi AI HP = 0
    if (aiHP == 0 || roundedHP == 0) {
        BITMAP_HP_0_ID_1.setVisible(true);
        BITMAP_HP_0_ID_1.invalidate();

        // FORCE END GAME khi AI chết
        if (!gameEnded) {
            endGame(true); // Player thắng
        }
        return;
    }

    // Hiển thị widget HP phù hợp cho AI (sử dụng các widget có _1)
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
                endGame(true); // Player thắng
            }
            break;
    }
}

void GameScreenView::hideAllPlayerHPWidgets()
{
    // Ẩn tất cả HP widgets của player
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

    // Invalidate tất cả
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
    // Ẩn tất cả HP widgets của AI
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

    // Invalidate tất cả
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
    // Shield blocks everything completely
    if (hasShield) {
        return 0; // Shield chặn hoàn toàn
    }

    int finalDamage = baseDamage; // SPECIAL KHÔNG ĐƯỢC DOUBLE DAMAGE

    // Apply defense reduction
    if (isDefending) {
        // Special attack khi bị defend: giảm xuống 25 damage
        finalDamage = 25;
    }

    return finalDamage;
}

// ==================== CHAKRA SYSTEM ====================

void GameScreenView::updatePlayerChakraDisplay()
{
    // Đảm bảo chakra level không bị âm
    if (playerChakraLevel < 0) playerChakraLevel = 0;
    if (playerChakraLevel > 100) playerChakraLevel = 100;

    // Xử lý đặc biệt khi chakra = 0
    if (playerChakraLevel == 0) {
        // Ẩn hoàn toàn chakra box
        chakra_box1.setVisible(false);
        chakra_box1.invalidate();
        return;
    }

    // Hiển thị lại chakra box nếu có chakra
    chakra_box1.setVisible(true);

    // Tính toán width dựa trên chakra level (0-100%)
    int maxWidth = 86; // Width tối đa của thanh chakra player
    int currentWidth = (playerChakraLevel * maxWidth) / 100;

    // Đảm bảo width hợp lệ
    if (currentWidth < 1) currentWidth = 1; // Tối thiểu 1 pixel nếu có chakra
    if (currentWidth > maxWidth) currentWidth = maxWidth;

    // Cập nhật kích thước Box chakra của player
    chakra_box1.setPosition(14, 43, currentWidth, 5);
    chakra_box1.invalidate();
}

void GameScreenView::updateAIChakraDisplay()
{
    // Đảm bảo AI chakra level không bị âm
    if (aiChakraLevel < 0) aiChakraLevel = 0;
    if (aiChakraLevel > 100) aiChakraLevel = 100;

    // Xử lý đặc biệt khi chakra = 0
    if (aiChakraLevel == 0) {
        // Ẩn hoàn toàn chakra box
        chakra_box2.setVisible(false);
        chakra_box2.invalidate();
        return;
    }

    // Hiển thị lại chakra box nếu có chakra
    chakra_box2.setVisible(true);

    // Tính toán width dựa trên AI chakra level (0-100%)
    int maxWidth = 86; // Width tối đa của thanh chakra AI
    int currentWidth = (aiChakraLevel * maxWidth) / 100;

    // Đảm bảo width hợp lệ
    if (currentWidth < 1) currentWidth = 1; // Tối thiểu 1 pixel nếu có chakra
    if (currentWidth > maxWidth) currentWidth = maxWidth;

    // Cập nhật kích thước Box chakra của AI
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

// ==================== GAME END SYSTEM ====================

void GameScreenView::checkGameEnd()
{
    // BỎ FUNCTION NÀY VÌ GIỜ KIỂM TRA TRỰC TIẾP TRONG updateHPDisplay()
    // Logic game end giờ được xử lý trong updatePlayerHPDisplay() và updateAIHPDisplay()
    return;
}

void GameScreenView::endGame(bool playerWon)
{
    gameEnded = true;

    // ===== LƯU KẾT QUẢ GAME CHO ROUTING =====
    playerWonGame = playerWon;

    // Hide menu actions immediately
    hideActionButtons();
    btn_arrow1.setVisible(false);
    btn_arrow1.invalidate();

    // Hide all power-up icons when game ends
    hideAllPlayerPowerUps();
    hideAllAIPowerUps();
    hideDamageText();

    // Hide all animations before showing results
    naruto.setVisible(false);
    naruto_atk.setVisible(false);
    naruto_def.setVisible(false);
    naruto_spe.setVisible(false);

    bleach.setVisible(false);
    bleach_atk.setVisible(false);
    bleach_def.setVisible(false);
    bleach_spe.setVisible(false);

    // Hide game over screens first
    gameover.setVisible(false);
    win.setVisible(false);

    if (playerWon) {
        // ===== PLAYER THẮNG - WILL GO TO BACKUPSCREEN =====

        // LED signal for character unlock
        for (int i = 0; i < 8; i++) {
            toggleLED();
        }

        // Player wins - AI dies
        naruto_lose.setVisible(false);
        bleach_win.setVisible(false);

        naruto_win.setVisible(true);
        bleach_lose.setVisible(true);
        win.setVisible(true);

        naruto_win.invalidate();
        bleach_lose.invalidate();
        win.invalidate();

        // LED blink rapidly for victory
        for (int i = 0; i < 6; i++) {
            toggleLED();
        }

        // Debug: 3 quick blinks = BackUpScreen route
        for (int i = 0; i < 3; i++) {
            toggleLED();
        }
    } else {
        // ===== PLAYER THUA - WILL GO TO MAINMENUSCREEN =====

        // Player loses - Naruto dies
        naruto_win.setVisible(false);
        bleach_lose.setVisible(false);

        naruto_lose.setVisible(true);
        bleach_win.setVisible(true);
        gameover.setVisible(true);

        naruto_lose.invalidate();
        bleach_win.invalidate();
        gameover.invalidate();

        // LED stays on for defeat
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);

        // Debug: 5 slow blinks = MainMenuScreen route
        for (int i = 0; i < 5; i++) {
            toggleLED();
        }
    }

    // ===== BẮT ĐẦU AUTO RETURN TIMER (6 GIÂY) =====
    autoReturnTimer = 0;
    autoReturnTimerActive = true;

    // Debug: LED signal that timer has started
    toggleLED();
    toggleLED();
    toggleLED();

    // Ensure gameEnded = true is set
    gameEnded = true;
}
