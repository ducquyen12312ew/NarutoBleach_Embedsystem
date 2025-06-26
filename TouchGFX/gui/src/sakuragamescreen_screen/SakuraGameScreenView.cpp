#include <gui/sakuragamescreen_screen/SakuraGameScreenView.hpp>
#include <touchgfx/Color.hpp>
#include "stm32f4xx_hal.h"
#include <gui/character_screen/CharacterView.hpp>
#include <gui/character1_screen/Character1View.hpp>

SakuraGameScreenView::SakuraGameScreenView()
{
    resetTimer = 0;
    resetTimerActive = false;
    randomSeed = 12345;

    fireAnimationTimer = 0;
    fireAnimationActive = false;
    fireAnimationState = 0;
    fireLoopCounter = 0;
    specialFireToggle = false;

    // ===== BOSS SYSTEM INITIALIZATION =====
    bossPhase = 1;
    bossMaxHP = 130; // Boss có 130 HP: 30 HP từ 3 mauboss + 100 HP bình thường
    bossCurrentHP = 130;
    rageMode = false;
    rageModeTimer = 0;

    easterEggActivated = false;
     easterEggTimer = 0;
}

void SakuraGameScreenView::setupScreen()
{
    SakuraGameScreenViewBase::setupScreen();
    initGPIO();

    // Improved random seed
    randomSeed = HAL_GetTick();
    if (randomSeed == 0) randomSeed = 12345;
    randomSeed = (randomSeed * 1664525 + 1013904223);

    // Initialize game state
    playerHP = 100;
    bossCurrentHP = bossMaxHP; // Boss bắt đầu với 130 HP
    playerChakraLevel = 0;
    aiChakraLevel = 0;
    menuOpen = false;
    gameEnded = false;

    // Initialize combat state
    playerDefending = false;
    aiDefending = false;
    playerSpecialUsed = false;
    aiSpecialUsed = false;

    // Initialize boss-specific variables
    bossPhase = 1;
    rageMode = false;
    rageModeTimer = 0;

    // Initialize power-up system (chỉ cho player)
    turnCounter = 0;
    playerPowerUpType = 0;
    aiPowerUpType = 0; // Boss không có power-up
    playerPowerUpActive = false;
    aiPowerUpActive = false; // Boss không dùng power-up
    playerActionDone = false;
    aiActionDone = false;

    // ===== BOSS KHÔNG CÓ PERSONALITY RANDOM - CHỈ ADAPTIVE =====
    aiPersonality = 0; // 3 = Adaptive Boss Mode

    // ===== TIMER SYSTEM =====
    resetTimer = 0;
    resetTimerActive = false;
    autoReturnTimer = 0;
    autoReturnTimerActive = false;

    // Hide all power-up icons when starting
    hideAllPlayerPowerUps();
    hideAllAIPowerUps(); // Boss không có power-up nhưng vẫn hide để đảm bảo

    // Update initial UI
    updatePlayerHPDisplay();
    updateBossHPDisplay(); // Sử dụng hàm mới cho boss
    updatePlayerChakraDisplay();
    updateAIChakraDisplay();

    // Ensure both characters are in normal state when starting
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

void SakuraGameScreenView::hideAllFireEffects()
{
    fire1.setVisible(false);
    fire2.setVisible(false);
    bleach_spe_fire.setVisible(false);

    fire1.invalidate();
    fire2.invalidate();
    bleach_spe_fire.invalidate();
}

void SakuraGameScreenView::startAttackFireAnimation()
{
    hideAllFireEffects();
    fireAnimationTimer = 0;
    fireAnimationActive = true;
    fireAnimationState = 1; // Bắt đầu với fire1

    // Hiển thị fire1
    fire1.setVisible(true);
    fire1.invalidate();

    // Debug LED
    toggleLED();
}

void SakuraGameScreenView::startSpecialFireAnimation()
{
    hideAllFireEffects();
    fireAnimationTimer = 0;
    fireAnimationActive = true;
    fireAnimationState = 3; // Special fire blink
    fireLoopCounter = 0;
    specialFireToggle = true; // Bắt đầu với visible

    // Bắt đầu với bleach_spe_fire visible
    bleach_spe_fire.setVisible(true);
    bleach_spe_fire.invalidate();

    // Debug LED pattern for special
    for (int i = 0; i < 4; i++) {
        toggleLED();
    }
}

void SakuraGameScreenView::updateFireAnimation()
{
    if (!fireAnimationActive) return;

    fireAnimationTimer += 16; // TouchGFX tick ~16ms

    switch (fireAnimationState) {
        case 1: // Fire1 đang hiển thị (cho attack)
            if (fireAnimationTimer >= FIRE_ANIMATION_DURATION) {
                // Chuyển sang fire2
                fire1.setVisible(false);
                fire2.setVisible(true);
                fire1.invalidate();
                fire2.invalidate();

                fireAnimationState = 2;
                fireAnimationTimer = 0;
                toggleLED(); // Debug
            }
            break;

        case 2: // Fire2 đang hiển thị (cho attack)
            if (fireAnimationTimer >= FIRE_ANIMATION_DURATION) {
                // Kết thúc animation
                hideAllFireEffects();
                fireAnimationActive = false;
                fireAnimationState = 0;
                fireAnimationTimer = 0;

                // SỬA: THAY aiHP THÀNH bossCurrentHP
                if (!gameEnded && bossCurrentHP > 0) {
                    aiActionDone = true;
                    toggleLED(); // Debug: animation complete
                }
            }
            break;

        case 3: // Special fire blink - CHỈ NHẤP NHÁY bleach_spe_fire
            // Toggle every FIRE_LOOP_INTERVAL (0.3 giây)
            if (fireAnimationTimer >= FIRE_LOOP_INTERVAL) {
                // Toggle bleach_spe_fire on/off
                specialFireToggle = !specialFireToggle;
                bleach_spe_fire.setVisible(specialFireToggle);
                bleach_spe_fire.invalidate();

                fireAnimationTimer = 0;
                fireLoopCounter += FIRE_LOOP_INTERVAL;

                // Debug LED mỗi toggle
                toggleLED();
            }

            // Kết thúc sau 3 giây
            if (fireLoopCounter >= 3000) { // 3000ms = 3 giây
                hideAllFireEffects();

                // ===== ẨN bleach_spe CUỐI CÙNG =====
                bleach_spe.setVisible(false);
                bleach_spe.invalidate();

                fireAnimationActive = false;
                fireAnimationState = 0;
                fireAnimationTimer = 0;
                fireLoopCounter = 0;

                // SỬA: THAY aiHP THÀNH bossCurrentHP
                if (!gameEnded && bossCurrentHP > 0) {
                    aiActionDone = true;
                    aiSpecialUsed = true;

                    // Debug: special animation complete
                    for (int i = 0; i < 5; i++) {
                        toggleLED();
                    }
                }
            }
            break;
    }
}

void SakuraGameScreenView::tearDownScreen()
{
    SakuraGameScreenViewBase::tearDownScreen();
}

void SakuraGameScreenView::initGPIO()
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

void SakuraGameScreenView::toggleLED()
{
    HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
}

void SakuraGameScreenView::hideActionButtons()
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

void SakuraGameScreenView::showActionButtons()
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

void SakuraGameScreenView::toggleActionMenu()
{
    if (menuOpen) {
        hideActionButtons();
    } else {
        showActionButtons();
    }
    toggleLED();
}

void SakuraGameScreenView::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED) {
    	if (!gameEnded && !easterEggActivated) {
    	            // Kiểm tra click vào vùng con rồng (bleach)
    	            // Vùng con rồng khoảng: x: 94-240, y: 62-234 (tùy thuộc vào sprite size)
    	            if (evt.getX() >= 94 && evt.getX() <= 240 &&
    	                evt.getY() >= 62 && evt.getY() <= 234) {

    	                // KÍCH HOẠT EASTER EGG
    	                activateEasterEgg();
    	                return;
    	            }
    	        }
        // DEBUG: Click góc trên trái để giảm boss HP
        if (evt.getX() < 50 && evt.getY() < 50) {
            // Simple debug: Giảm HP 30 điểm mỗi lần click
            bossCurrentHP -= 30;
            if (bossCurrentHP < 0) bossCurrentHP = 0;
            updateBossHPDisplay();
            return;
        }

        // DEBUG: Click góc trên phải để force kill PLAYER
        if (evt.getX() > 190 && evt.getY() < 50) {
            playerHP = 0;
            updatePlayerHPDisplay();
            return;
        }

        // DEBUG: Click góc dưới trái để kiểm tra game state
        if (evt.getX() < 50 && evt.getY() > 270) {
            // LED pattern đơn giản để debug
            for (int i = 0; i < 3; i++) {
                toggleLED();
            }
            return;
        }

        // DEBUG: Click góc dưới phải để GUARANTEED power-up cho player
        if (evt.getX() > 190 && evt.getY() > 270 && !gameEnded) {
            givePlayerPowerUp(); // Chỉ player nhận power-up
            return;
        }

        // DEBUG: Click giữa dưới để xem turn counter + force complete turn
        if (evt.getX() > 100 && evt.getX() < 140 && evt.getY() > 270 && !gameEnded) {
            // LED blink theo số turn hiện tại
            for (int i = 0; i < turnCounter + 1; i++) {
                toggleLED();
            }

            // Force complete turn
            playerActionDone = true;
            aiActionDone = true;
            checkTurnComplete();
            return;
        }

        // Game logic bình thường
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

    SakuraGameScreenViewBase::handleClickEvent(evt);
}


void SakuraGameScreenView::handleDragEvent(const DragEvent& evt)
{
    SakuraGameScreenViewBase::handleDragEvent(evt);
}

void SakuraGameScreenView::handleTickEvent()
{
    // FORCE REFRESH BOSS HP MỖI FRAME ĐẦU TIÊN
    static int frameCounter = 0;
    if (frameCounter < 10 && !gameEnded) {
        frameCounter++;
        updateBossHPDisplay();

        // Extra debug LED
        if (frameCounter == 5) {
            toggleLED();
            toggleLED();
            toggleLED();
        }
    }

    // ===== XỬ LÝ EASTER EGG TIMER TRƯỚC TIÊN =====
    if (easterEggActivated) {
        easterEggTimer += 16; // TouchGFX tick ~16ms

        // Sau 3 giây, chuyển thẳng về WIN_SCENE
        if (easterEggTimer >= EASTER_EGG_WIN_TIME) {
            // Tắt LED trước khi chuyển screen
            HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);

            // CHUYỂN VỀ WIN_SCENE (thay đổi tên screen tùy project)
            application().gotoCharacter1ScreenNoTransition();
            return;
        }
        return; // Không xử lý logic game khác khi Easter Egg active
    }

    // Only process timers when game hasn't ended
    if (!gameEnded) {
        // Handle auto reset timer
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

        // ===== UPDATE RAGE MODE TIMER =====
        if (rageMode) {
            rageModeTimer += 16;
            // Có thể thêm effect visual cho rage mode ở đây
        }

        // Update fire animation
        updateFireAnimation();

    } else {
        // Auto return timer when game ended (NORMAL END GAME)
        if (autoReturnTimerActive) {
            autoReturnTimer += 16;

            if (autoReturnTimer >= AUTO_RETURN_TIME_MS) {
                HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
                application().gotoBackUpScreenScreenNoTransition();
                return;
            }
        }
    }

    SakuraGameScreenViewBase::handleTickEvent();
}

void SakuraGameScreenView::hideAllBossHPWidgets()
{
    // Hide all boss HP widgets (bao gồm cả 130, 120, 110)
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

    // Invalidate all
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

void SakuraGameScreenView::forceBossHPVisible()
{
    // FORCE SHOW BITMAP_HP_130_ID_1 khi bắt đầu game
    if (bossCurrentHP >= 125) { // Boss bắt đầu với 130 HP
        BITMAP_HP_130_ID_1.setVisible(true);
        BITMAP_HP_130_ID_1.setAlpha(255);
        BITMAP_HP_130_ID_1.invalidate();

        // Debug confirmation
        toggleLED();
        toggleLED();
        toggleLED();
        toggleLED();
        toggleLED();
    }
}
// ==================== POWER-UP SYSTEM ====================


void SakuraGameScreenView::checkTurnComplete()
{
    if (playerActionDone && aiActionDone) {
        turnCounter++;

        toggleLED();

        // ===== ONLY PLAYER GETS POWER-UPS =====
        if (!playerSpecialUsed && !aiSpecialUsed) {
            // TĂNG CHANCE POWER-UP CHO PLAYER KHI ĐẤU BOSS: 25% thay vì 20%
            int powerUpChance = generateRandomNumber(1, 100);
            if (powerUpChance <= 25) {
                givePlayerPowerUp(); // Chỉ player nhận power-up
            }
        }

        // Clear player power-ups after use
        if (playerPowerUpActive && (playerPowerUpType == 2 || playerPowerUpType == 3 || playerPowerUpType == 4)) {
            clearPowerUpAfterUse(true);
        }

        // Boss không có power-up để clear

        // Reset
        playerActionDone = false;
        aiActionDone = false;
        playerSpecialUsed = false;
        aiSpecialUsed = false;
    }
}

void SakuraGameScreenView::givePlayerPowerUp()
{
    // ENHANCED POWER-UP FOR BOSS FIGHT
    for (int i = 0; i < 3; i++) {
        toggleLED();
    }

    // ===== ENHANCED DISTRIBUTION WHEN FIGHTING BOSS =====
    // Shield 30%, X2Dame 25%, Healthplus 25%, Chakragain 20%

    int playerRoll = generateRandomNumber(1, 100);
    int newPlayerPowerUp;

    if (playerRoll >= 1 && playerRoll <= 30) {
        newPlayerPowerUp = 4; // 30% - Shield
    } else if (playerRoll >= 31 && playerRoll <= 55) {
        newPlayerPowerUp = 1; // 25% - Double Damage
    } else if (playerRoll >= 56 && playerRoll <= 80) {
        newPlayerPowerUp = 2; // 25% - Heal (ENHANCED)
    } else {
        newPlayerPowerUp = 3; // 20% - Chakra
    }

    playerPowerUpType = newPlayerPowerUp;
    playerPowerUpActive = true;
    applyInstantPowerUp(true, playerPowerUpType);
    updatePlayerPowerUpDisplay();
}

void SakuraGameScreenView::applyInstantPowerUp(bool isPlayer, int type)
{
    if (!isPlayer) return; // Boss không có power-up

    switch (type) {
        case 1: // Double Damage - chờ đến khi combat
            break;

        case 2: // Heal - ENHANCED CHO BOSS FIGHT
            playerHP += 8; // TĂNG TỪ 5 LÊN 8
            if (playerHP > 100) playerHP = 100;
            updatePlayerHPDisplay();
            break;

        case 3: // Chakra - ENHANCED CHO BOSS FIGHT
            playerChakraLevel += 20; // TĂNG TỪ 15 LÊN 20
            if (playerChakraLevel > 100) playerChakraLevel = 100;
            updatePlayerChakraDisplay();
            break;

        case 4: // Shield
            break;
    }
}


void SakuraGameScreenView::updatePlayerPowerUpDisplay()
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

void SakuraGameScreenView::updateAIPowerUpDisplay(){
}


void SakuraGameScreenView::hideAllPlayerPowerUps()
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

void SakuraGameScreenView::hideAllAIPowerUps()
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

void SakuraGameScreenView::clearPowerUpAfterUse(bool isPlayer)
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

int SakuraGameScreenView::generateRandomNumber(int min, int max)
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
int SakuraGameScreenView::calculateDamage(int baseDamage, bool isDefending, bool hasDoubleAttack, bool hasShield)
{
    // Shield blocks everything completely (chỉ có Shield mới block 100%)
    if (hasShield) {
        return 0; // Shield chặn hoàn toàn
    }

    int finalDamage = baseDamage;

    // Apply double damage first
    if (hasDoubleAttack) {
        finalDamage = (int)(baseDamage * 2.5); // x2.5 như yêu cầu
    }

    // ===== SỬA: DEFEND CHỈ GIẢM 50% DAMAGE, KHÔNG BLOCK HOÀN TOÀN =====
    if (isDefending) {
        // Defend giảm 50% damage cho tất cả các loại tấn công
        finalDamage = finalDamage / 2;

        // Đảm bảo tối thiểu 1 damage (không thể defend xuống 0)
        if (finalDamage < 1) finalDamage = 1;
    }

    return finalDamage;
}

// ==================== PLAYER COMBAT FUNCTIONS ====================

void SakuraGameScreenView::playerAttack()
{
    resetNarutoState();
    naruto.setVisible(false);
    naruto_atk.setVisible(true);
    naruto_atk.invalidate();
    naruto.invalidate();

    // Player damage calculation (unchanged)
    int baseDamage = generateRandomNumber(3, 8);
    bool hasDoubleAttack = (playerPowerUpActive && playerPowerUpType == 1);
    int actualDamage = calculateDamage(baseDamage, aiDefending, hasDoubleAttack, false); // Boss không có shield

    // Apply damage to boss
    bossCurrentHP -= actualDamage;
    if (bossCurrentHP < 0) bossCurrentHP = 0;

    // Gain chakra
    playerChakraLevel += generateRandomNumber(5, 10);
    if (playerChakraLevel > 100) playerChakraLevel = 100;

    // Clear power-ups
    if (hasDoubleAttack) {
        clearPowerUpAfterUse(true);
    }

    // Update UI
    updateBossHPDisplay(); // Sử dụng hàm boss mới
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

void SakuraGameScreenView::playerDefend()
{
    resetNarutoState();
    naruto.setVisible(false);
    naruto_def.setVisible(true);
    naruto_def.invalidate();
    naruto.invalidate();

    // Set defend state
    playerDefending = true;

    // ENHANCED healing khi đấu boss
    int healAmount = generateRandomNumber(3, 7); // TĂNG TỪ 2-5 LÊN 3-7
    playerHP += healAmount;
    if (playerHP > 100) playerHP = 100;

    // ENHANCED chakra gain
    playerChakraLevel += generateRandomNumber(10, 18); // TĂNG TỪ 8-15 LÊN 10-18
    if (playerChakraLevel > 100) playerChakraLevel = 100;

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

void SakuraGameScreenView::playerSpecial()
{
    if (gameEnded || playerChakraLevel < 100) return;

    resetNarutoState();
    naruto.setVisible(false);
    naruto_spe.setVisible(true);
    naruto_spe.invalidate();
    naruto.invalidate();

    // ===== NARUTO HEALING + SHIELD SPECIAL FOR BOSS FIGHT =====

    // 1. HỒI ĐẦY MÁU (100 HP)
    int oldHP = playerHP;
    playerHP = 100; // Full heal
    int healedAmount = playerHP - oldHP;

    // 2. TẠO SHIELD TẠM THỜI (chống 1 đòn tấn công tiếp theo)
    if (!playerPowerUpActive) {
        // Tạo shield power-up mới
        playerPowerUpType = 4; // Shield
        playerPowerUpActive = true;
        updatePlayerPowerUpDisplay();
    } else if (playerPowerUpActive && playerPowerUpType != 4) {
        // Nếu đã có power-up khác, thay thế bằng shield
        clearPowerUpAfterUse(true); // Clear power-up cũ
        playerPowerUpType = 4; // Set shield
        playerPowerUpActive = true;
        updatePlayerPowerUpDisplay();
    }
    // Nếu đã có shield rồi thì không làm gì (không stack shield)

    // Reset chakra về 0 (vẫn tiêu tốn chakra như special skill bình thường)
    playerChakraLevel = 0;

    // Update UI
    updatePlayerHPDisplay();
    updatePlayerChakraDisplay();
    updatePlayerPowerUpDisplay(); // Đảm bảo shield được hiển thị

    // LED effect đặc biệt cho healing + shield special
    for (int i = 0; i < 8; i++) {
        toggleLED(); // 8 LED blinks để báo hiệu healing + shield
    }

    playerActionDone = true;
    playerSpecialUsed = true; // Đánh dấu đã dùng special (không có power-up generation)

    // Debug: thêm LED để confirm đã nhận shield
    if (playerPowerUpActive && playerPowerUpType == 4) {
        for (int i = 0; i < 3; i++) {
            toggleLED(); // 3 LED blinks thêm để confirm shield
        }
    }

    toggleLED();
    startResetTimer();
}

// ==================== AI COMBAT FUNCTIONS ====================

void SakuraGameScreenView::performAIAction()
{
    // KIỂM TRA QUAN TRỌNG: Nếu game đã kết thúc hoặc AI đã chết thì KHÔNG làm gì
	if (gameEnded || bossCurrentHP <= 0) {
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
    	int aiHPPercent = (bossCurrentHP * 100) / bossMaxHP;
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

            case 3: // Boss Adaptive Mode - MỚI THÊM
                   if (aiChakraLevel >= 100) {
                       // Boss có chakra đầy: ưu tiên special hoặc defend tùy tình huống
                       int bossHPPercent = (bossCurrentHP * 100) / bossMaxHP;
                       bool playerCanSpecial = (playerChakraLevel >= 100);

                       if (bossHPPercent <= 30) {
                           // Boss HP thấp: 70% special, 30% defend
                           if (generateRandomNumber(1, 100) <= 70) {
                               action = 3; // Special
                           } else {
                               action = 2; // Defend
                           }
                       } else if (playerCanSpecial) {
                           // Player có thể special: 40% defend, 60% special
                           if (generateRandomNumber(1, 100) <= 40) {
                               action = 2; // Defend
                           } else {
                               action = 3; // Special
                           }
                       } else {
                           // Tình huống bình thường: 50% special, 50% attack
                           if (generateRandomNumber(1, 100) <= 50) {
                               action = 3; // Special
                           } else {
                               action = 1; // Attack
                           }
                       }
                   } else {
                       // Boss không có chakra: chọn giữa attack và defend
                       int bossHPPercent = (bossCurrentHP * 100) / bossMaxHP;
                       bool playerCanSpecial = (playerChakraLevel >= 100);

                       if (bossHPPercent <= 50) {
                           // Boss HP thấp: ưu tiên defend để hồi máu
                           if (generateRandomNumber(1, 100) <= 70) {
                               action = 2; // Defend
                           } else {
                               action = 1; // Attack
                           }
                       } else if (playerCanSpecial) {
                           // Player có thể special: tăng defend
                           if (generateRandomNumber(1, 100) <= 60) {
                               action = 2; // Defend
                           } else {
                               action = 1; // Attack
                           }
                       } else {
                           // Tình huống bình thường: 60% attack, 40% defend
                           if (generateRandomNumber(1, 100) <= 60) {
                               action = 1; // Attack
                           } else {
                               action = 2; // Defend
                           }
                       }
                   }
                   break;

               default:
                   // Fallback: dùng logic aggressive
                   action = 1; // Attack
                   break;
           }
        }


    // Thực hiện hành động - VỚI KIỂM TRA AN TOÀN
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

void SakuraGameScreenView::aiAttack()
{
    if (gameEnded) return;

    resetBleachState();

    bleach.setVisible(false);
    bleach_atk.setVisible(true);
    bleach_atk.invalidate();
    bleach.invalidate();

    // ===== BOSS DAMAGE CALCULATION =====
    int baseDamage;
    switch(bossPhase) {
        case 1: // Phase 1 (130-81): 4-10 damage
            baseDamage = generateRandomNumber(4, 10);
            break;
        case 2: // Phase 2 (80-51): 6-12 damage
            baseDamage = generateRandomNumber(6, 12);
            break;
        case 3: // Phase 3 (50-21) + Rage: 8-15 damage
            baseDamage = generateRandomNumber(8, 15);
            break;
        case 4: // Phase 4 (20-1) + Rage Max: 10-18 damage
            baseDamage = generateRandomNumber(10, 18);
            break;
        default:
            baseDamage = generateRandomNumber(4, 10);
            break;
    }

    // Boss không có power-up, chỉ check player shield
    bool playerHasShield = (playerPowerUpActive && playerPowerUpType == 4);
    int actualDamage = calculateDamage(baseDamage, playerDefending, false, playerHasShield);

    // Apply damage to player
    playerHP -= actualDamage;
    if (playerHP < 0) playerHP = 0;

    // ===== SỬA: BOSS GAIN CHAKRA CHẬM HỚN =====
    int chakraGain = generateRandomNumber(4, 8); // GIẢM TỪ 8-15 XUỐNG 4-8
    aiChakraLevel += chakraGain;
    if (aiChakraLevel > 100) aiChakraLevel = 100;

    // Clear player shield if it blocked damage
    if (playerHasShield && actualDamage == 0) {
        clearPowerUpAfterUse(true);
    }

    // Update UI
    updatePlayerHPDisplay();
    updateAIChakraDisplay();

    // Start fire animation instead of ending immediately
    startAttackFireAnimation();
    startResetTimer();
}

void SakuraGameScreenView::aiDefend()
{
    if (gameEnded) return;

    resetBleachState();

    bleach.setVisible(false);
    bleach_def.setVisible(true);
    bleach_def.invalidate();
    bleach.invalidate();

    // Set defend state
    aiDefending = true;

    // Boss heal amount based on phase
    int healAmount;
    switch(bossPhase) {
        case 1: // Phase 1 (130-81): 2-5 heal
            healAmount = generateRandomNumber(2, 5);
            break;
        case 2: // Phase 2 (80-51): 3-6 heal
            healAmount = generateRandomNumber(3, 6);
            break;
        case 3: // Phase 3 (50-21) + Rage: 4-7 heal
            healAmount = generateRandomNumber(4, 7);
            break;
        case 4: // Phase 4 (20-1) + Rage Max: 5-8 heal
            healAmount = generateRandomNumber(5, 8);
            break;
        default:
            healAmount = generateRandomNumber(2, 5);
            break;
    }

    bossCurrentHP += healAmount;
    if (bossCurrentHP > bossMaxHP) bossCurrentHP = bossMaxHP;

    // ===== SỬA: BOSS GAIN CHAKRA CHẬM HỚN KHI DEFEND =====
    aiChakraLevel += generateRandomNumber(8, 12); // GIẢM TỪ 12-20 XUỐNG 8-12
    if (aiChakraLevel > 100) aiChakraLevel = 100;

    // Update UI
    updateBossHPDisplay();
    updateAIChakraDisplay();

    aiActionDone = true;
}

void SakuraGameScreenView::aiSpecial()
{
    // Boss special requirements based on phase
    int requiredChakra = 100;
    if (rageMode && bossPhase >= 3) {
        requiredChakra = 80; // Rage mode: chỉ cần 80 chakra
    }

    if (aiChakraLevel < requiredChakra || gameEnded) {
        return;
    }

    resetBleachState();

    bleach.setVisible(false);
    bleach_spe.setVisible(true);
    bleach_spe.invalidate();
    bleach.invalidate();

    // ===== SỬA: BOSS SPECIAL DAMAGE CỐ ĐỊNH 70 =====
    int specialDamage = 70; // LUÔN LUÔN GÂY 70 DAMAGE

    bool playerHasShield = (playerPowerUpActive && playerPowerUpType == 4);
    int actualDamage = calculateSpecialDamage(specialDamage, playerDefending, playerHasShield);

    // Apply damage to player
    playerHP -= actualDamage;
    if (playerHP < 0) playerHP = 0;

    // Reset chakra
    aiChakraLevel = 0;

    // Clear player shield if it blocked damage
    if (playerHasShield && actualDamage == 0) {
        clearPowerUpAfterUse(true);
    }

    // Update UI
    updateAIChakraDisplay();
    updatePlayerHPDisplay();

    // Start special fire animation
    startSpecialFireAnimation();
    startResetTimer();
}

// ==================== CHARACTER STATE FUNCTIONS ====================

void SakuraGameScreenView::resetNarutoState()
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

void SakuraGameScreenView::resetBleachState()
{
    // Ẩn tất cả các trạng thái đặc biệt của bleach
    bleach_atk.setVisible(false);
    bleach_def.setVisible(false);
    bleach_spe.setVisible(false);
    bleach_lose.setVisible(false);
    bleach_win.setVisible(false);

    // SỬA: THAY aiHP THÀNH bossCurrentHP
    if (bossCurrentHP > 0 && !gameEnded) {
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

void SakuraGameScreenView::startResetTimer()
{
    resetTimer = 0;
    resetTimerActive = true;
}

// ==================== HP DISPLAY SYSTEM ====================

void SakuraGameScreenView::updatePlayerHPDisplay()
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

void SakuraGameScreenView::hideAllPlayerHPWidgets()
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

int SakuraGameScreenView::calculateSpecialDamage(int baseDamage, bool isDefending, bool hasShield)
{
    // Shield blocks everything completely
    if (hasShield) {
        return 0; // Shield chặn hoàn toàn
    }

    // ===== SỬA: SPECIAL DAMAGE CỐ ĐỊNH 70, CHỈ BỊ GIẢM KHI DEFEND =====
    int finalDamage = baseDamage; // baseDamage = 70 cố định

    // Defend giảm 50% damage (70 -> 35)
    if (isDefending) {
        finalDamage = finalDamage / 2; // 70 / 2 = 35

        // Đảm bảo tối thiểu 1 damage
        if (finalDamage < 1) finalDamage = 1;
    }

    return finalDamage;
}

// ==================== CHAKRA SYSTEM ====================

void SakuraGameScreenView::updatePlayerChakraDisplay()
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

void SakuraGameScreenView::updateAIChakraDisplay()
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

void SakuraGameScreenView::gainChakra(int amount)
{
    playerChakraLevel += amount;
    if (playerChakraLevel > 100)
    {
        playerChakraLevel = 100;
    }

    updatePlayerChakraDisplay();
}

void SakuraGameScreenView::useChakra(int amount)
{
    playerChakraLevel -= amount;
    if (playerChakraLevel < 0)
    {
        playerChakraLevel = 0;
    }

    updatePlayerChakraDisplay();
}

// ==================== GAME END SYSTEM ====================

void SakuraGameScreenView::checkGameEnd()
{
    // BỎ FUNCTION NÀY VÌ GIỜ KIỂM TRA TRỰC TIẾP TRONG updateHPDisplay()
    // Logic game end giờ được xử lý trong updatePlayerHPDisplay() và updateAIHPDisplay()
    return;
}

void SakuraGameScreenView::endGame(bool playerWon)
{
    gameEnded = true;

    // Hide menu actions immediately
    hideActionButtons();
    btn_arrow1.setVisible(false);
    btn_arrow1.invalidate();

    // Hide all power-up icons when game ends
    hideAllPlayerPowerUps();

    // FORCE STOP ALL ANIMATIONS
    fireAnimationActive = false;
    fireAnimationTimer = 0;
    resetTimerActive = false;
    resetTimer = 0;

    // ===== FORCE HIDE ALL FIRE EFFECTS =====
    hideAllFireEffects();

    // ===== FORCE HIDE ALL CHARACTER SPRITES =====
    // Naruto sprites
    naruto.setVisible(false);
    naruto_atk.setVisible(false);
    naruto_def.setVisible(false);
    naruto_spe.setVisible(false);
    naruto_lose.setVisible(false);
    naruto_win.setVisible(false);

    // Bleach sprites - FORCE HIDE ALL INCLUDING bleach_spe
    bleach.setVisible(false);
    bleach_atk.setVisible(false);
    bleach_def.setVisible(false);
    bleach_spe.setVisible(false);  // ĐẶC BIỆT ẨN CÁI NÀY
    bleach_lose.setVisible(false);
    bleach_win.setVisible(false);

    // Hide game over screens first
    gameover.setVisible(false);
    win.setVisible(false);

    // ===== INVALIDATE ALL TO FORCE REFRESH =====
    naruto.invalidate();
    naruto_atk.invalidate();
    naruto_def.invalidate();
    naruto_spe.invalidate();
    naruto_lose.invalidate();
    naruto_win.invalidate();

    bleach.invalidate();
    bleach_atk.invalidate();
    bleach_def.invalidate();
    bleach_spe.invalidate();  // QUAN TRỌNG
    bleach_lose.invalidate();
    bleach_win.invalidate();

    if (playerWon) {
        // LED signal for character unlock
        for (int i = 0; i < 8; i++) {
            toggleLED();
        }

        // ===== CHỈ HIỂN THỊ CÁC SPRITE CẦN THIẾT =====
        naruto_win.setVisible(true);
        bleach_lose.setVisible(true);  // CHỈ CON RỒNG NẰM
        win.setVisible(true);

        // FORCE REFRESH ONLY NEEDED SPRITES
        naruto_win.invalidate();
        bleach_lose.invalidate();
        win.invalidate();

        // LED blink rapidly for victory
        for (int i = 0; i < 6; i++) {
            toggleLED();
        }
    } else {
        // Player loses - Naruto dies
        naruto_lose.setVisible(true);
        bleach_win.setVisible(true);
        gameover.setVisible(true);

        naruto_lose.invalidate();
        bleach_win.invalidate();
        gameover.invalidate();

        // LED stays on for defeat
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);

        // ===== PLAYER THUA: GOTO BACKUPSCREEN NGAY LẬP TỨC =====
        // Tắt LED trước khi chuyển screen
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);

        // Chuyển về BackUpScreen ngay lập tức
        application().gotoBackUpScreenScreenNoTransition();
        return;
    }

    // ===== CHỈ KHI PLAYER THẮNG MỚI CÓ AUTO RETURN TIMER =====
    autoReturnTimer = 0;
    autoReturnTimerActive = true;

    // Debug: LED signal that timer has started (chỉ khi player thắng)
    toggleLED();
    toggleLED();
    toggleLED();

    // Ensure gameEnded = true is set
    gameEnded = true;
}

void SakuraGameScreenView::activateEasterEgg()
{
    // Set flag
    easterEggActivated = true;
    easterEggTimer = 0;
    gameEnded = true; // Kết thúc game ngay lập tức

    // FORCE STOP ALL ANIMATIONS
    fireAnimationActive = false;
    fireAnimationTimer = 0;
    resetTimerActive = false;
    resetTimer = 0;

    // ===== FORCE HIDE ALL FIRE EFFECTS =====
    hideAllFireEffects();

    // Hide menu actions immediately
    hideActionButtons();
    btn_arrow1.setVisible(false);
    btn_arrow1.invalidate();

    // Hide all power-up icons
    hideAllPlayerPowerUps();

    // ===== FORCE HIDE ALL CHARACTER SPRITES =====
    naruto.setVisible(false);
    naruto_atk.setVisible(false);
    naruto_def.setVisible(false);
    naruto_spe.setVisible(false);
    naruto_lose.setVisible(false);
    naruto_win.setVisible(false);

    bleach.setVisible(false);
    bleach_atk.setVisible(false);
    bleach_def.setVisible(false);
    bleach_spe.setVisible(false);  // QUAN TRỌNG: ẨN CÁI NÀY
    bleach_lose.setVisible(false);
    bleach_win.setVisible(false);

    // Hide game over screens first
    gameover.setVisible(false);
    win.setVisible(false);

    // ===== INVALIDATE ALL =====
    naruto.invalidate();
    naruto_atk.invalidate();
    naruto_def.invalidate();
    naruto_spe.invalidate();
    naruto_lose.invalidate();
    naruto_win.invalidate();

    bleach.invalidate();
    bleach_atk.invalidate();
    bleach_def.invalidate();
    bleach_spe.invalidate();  // QUAN TRỌNG
    bleach_lose.invalidate();
    bleach_win.invalidate();

    // ===== CHỈ HIỂN THỊ SPRITE CẦN THIẾT =====
    naruto_win.setVisible(true);
    bleach_lose.setVisible(true);  // CHỈ CON RỒNG NẰM
    win.setVisible(true);

    naruto_win.invalidate();
    bleach_lose.invalidate();
    win.invalidate();

    // ===== EASTER EGG LED PATTERN =====
    for (int i = 0; i < 15; i++) {
        toggleLED(); // 15 LED blinks để báo hiệu Easter Egg
    }

    // Debug: LED signal that Easter Egg activated
    toggleLED();
    toggleLED();
    toggleLED();
}

void SakuraGameScreenView::updateBossHPDisplay()
{
    // Hide all boss HP widgets first
    hideAllBossHPWidgets();

    // ===== HỆ THỐNG HP ĐỐN GIẢN - GIỐNG GAMESCREEN NHƯNG BẮT ĐẦU TỪ 130 =====
    // Làm tròn xuống bội số 10 (giống GameScreenView)
    int roundedHP = (bossCurrentHP / 10) * 10;
    if (roundedHP < 0) roundedHP = 0;
    if (roundedHP > 130) roundedHP = 130;

    // DEBUG: Force test khi Boss HP = 0
    if (bossCurrentHP == 0 || roundedHP == 0) {
        // FORCE HIỂN THỊ HP_0_ID_1
        BITMAP_HP_0_ID_1.setVisible(true);
        BITMAP_HP_0_ID_1.invalidate();

        // DEBUG: LED blink nhiều lần
        for (int i = 0; i < 10; i++) {
            toggleLED();
        }

        // FORCE END GAME
        if (!gameEnded) {
            endGame(true); // Player thắng
        }
        return;
    }

    // Hiển thị widget HP phù hợp cho boss
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
            // Fallback case - cũng force về 0
            BITMAP_HP_0_ID_1.setVisible(true);
            BITMAP_HP_0_ID_1.invalidate();
            if (!gameEnded) {
                endGame(true); // Player thắng
            }
            break;
    }
}

