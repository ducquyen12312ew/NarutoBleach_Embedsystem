#ifndef NARUTOGAMESCREEN2VIEW_HPP
#define NARUTOGAMESCREEN2VIEW_HPP

#include <gui_generated/narutogamescreen2_screen/NarutoGameScreen2ViewBase.hpp>
#include <gui/narutogamescreen2_screen/NarutoGameScreen2Presenter.hpp>

class NarutoGameScreen2View : public NarutoGameScreen2ViewBase
{
public:
    NarutoGameScreen2View();
    virtual ~NarutoGameScreen2View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);
    virtual void handleTickEvent();

    // ===== MENU FUNCTIONS =====
    void hideActionButtons();
    void showActionButtons();
    void toggleActionMenu();

    // ===== PLAYER COMBAT FUNCTIONS =====
    void playerAttack();
    void playerDefend();
    void playerSpecial();

    // ===== BOSS AI FUNCTIONS =====
    void performAIAction();
    void aiAttack();
    void aiDefend();
    void aiSpecial();

    // ===== DAMAGE CALCULATION =====
    int calculateDamage(int baseDamage, bool isDefending, bool hasDoubleAttack, bool hasShield);
    int calculateSpecialDamage(int baseDamage, bool isDefending, bool hasShield);

    // ===== HP DISPLAY SYSTEM =====
    void updatePlayerHPDisplay();
    void updateBossHPDisplay();
    void hideAllPlayerHPWidgets();
    void hideAllBossHPWidgets();

    // ===== CHAKRA SYSTEM =====
    void updatePlayerChakraDisplay();
    void updateAIChakraDisplay();
    void gainChakra(int amount);
    void useChakra(int amount);

    // ===== CHARACTER STATE FUNCTIONS =====
    void resetNarutoState();
    void resetBleachState();
    void startResetTimer();

    // ===== GAME END SYSTEM =====
    void checkGameEnd();
    void endGame(bool playerWon);

    // ===== POWER-UP SYSTEM (PLAYER ONLY) =====
    void checkTurnComplete();
    void givePlayerPowerUp();
    void applyInstantPowerUp(bool isPlayer, int type);
    void updatePlayerPowerUpDisplay();
    void updateAIPowerUpDisplay();
    void hideAllPlayerPowerUps();
    void hideAllAIPowerUps();
    void clearPowerUpAfterUse(bool isPlayer);

    // ===== FIRE ANIMATION SYSTEM =====
    void hideAllFireEffects();
    void startAttackFireAnimation();
    void startSpecialFireAnimation();
    void updateFireAnimation();

    // ===== UTILITY FUNCTIONS =====
    int generateRandomNumber(int min, int max);

    // ===== GPIO FUNCTIONS =====
    void initGPIO();
    void toggleLED();

    // ===== DEBUG FUNCTIONS =====
    void debugGameState();
    void forceBossHPVisible();
    void debugBossHP();

    void activateEasterEgg();

protected:
    // ===== CORE GAME STATE =====
    int playerHP;                      // Player HP (0-100)
    bool menuOpen;                     // Action menu state
    bool gameEnded;                    // Game end flag

    // ===== BOSS SYSTEM =====
    int bossPhase;                     // 1-4 representing boss phases
    int bossMaxHP;                     // Maximum boss HP (130)
    int bossCurrentHP;                 // Current boss HP
    bool rageMode;                     // True when boss is in rage mode (Phase 3+)
    int rageModeTimer;                 // Timer for rage mode effects

    // ===== CHAKRA SYSTEM =====
    int playerChakraLevel;             // Player chakra (0-100)
    int aiChakraLevel;                 // Boss chakra (0-100)

    // ===== COMBAT STATE =====
    bool playerDefending;              // Player defend state
    bool aiDefending;                  // Boss defend state
    bool playerSpecialUsed;            // Player used special this turn
    bool aiSpecialUsed;                // Boss used special this turn

    // ===== AI SYSTEM =====
    int aiPersonality;                 // 3 = Adaptive Boss Mode (no random personalities)

    // ===== TIMER SYSTEM =====
    int resetTimer;                    // Timer for character state reset
    bool resetTimerActive;             // Reset timer active flag
    int autoReturnTimer;               // Timer for auto return to menu (6 seconds)
    bool autoReturnTimerActive;        // Auto return timer active flag
    static const int RESET_TIME_MS = 1000;           // 1 second for character reset
    static const int AUTO_RETURN_TIME_MS = 6000;     // 6 seconds delay before returning to menu

    // ===== POWER-UP SYSTEM (PLAYER ONLY) =====
    int turnCounter;                   // Turn counter for power-up generation
    int playerPowerUpType;             // Player power-up type (0=none, 1=damage, 2=heal, 3=chakra, 4=shield)
    int aiPowerUpType;                 // Keep for compatibility (always 0 for boss)
    bool playerPowerUpActive;          // Player power-up active flag
    bool aiPowerUpActive;              // Keep for compatibility (always false for boss)
    bool playerActionDone;             // Player completed action this turn
    bool aiActionDone;                 // Boss completed action this turn

    // ===== FIRE ANIMATION SYSTEM =====
    int fireAnimationTimer;            // Timer for fire animations
    bool fireAnimationActive;          // Fire animation active flag
    int fireAnimationState;            // Fire animation state (0=none, 1=fire1, 2=fire2, 3=special_fire_blink)
    int fireLoopCounter;               // Counter for fire animation loops
    bool specialFireToggle;            // Toggle on/off for special fire effect
    static const int FIRE_ANIMATION_DURATION = 500;  // 0.5 seconds per fire frame
    static const int FIRE_LOOP_INTERVAL = 300;       // 0.3 seconds toggle for special fire

    // ===== UTILITY VARIABLES =====
    uint32_t randomSeed;               // Seed for random number generation

    bool easterEggActivated;           // Flag for Easter Egg
    int easterEggTimer;                // Timer for Easter Egg (3 seconds)
    static const int EASTER_EGG_WIN_TIME = 3000;  // 3 seconds then switch to WIN_SCENE
};

#endif // NARUTOGAMESCREEN2VIEW_HPP
