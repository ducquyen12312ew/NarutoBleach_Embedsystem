#ifndef GAMESCREEN_VIEW_HPP
#define GAMESCREEN_VIEW_HPP

#include <gui_generated/gamescreen_screen/GameScreenViewBase.hpp>
#include <gui/gamescreen_screen/GameScreenPresenter.hpp>

class GameScreenView : public GameScreenViewBase
{
public:
    GameScreenView();
    virtual ~GameScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);
    virtual void handleTickEvent();

    // Menu functions
    void hideActionButtons();
    void showActionButtons();
    void toggleActionMenu();

    // Combat functions
    void playerAttack();
    void playerDefend();
    void playerSpecial();

    // AI functions
    void performAIAction();
    void aiAttack();
    void aiDefend();
    void aiSpecial();

    // Updated calculateDamage với power-up support
    int calculateDamage(int baseDamage, bool isDefending, bool hasDoubleAttack, bool hasShield);

    // HP display functions
    void updatePlayerHPDisplay();
    void updateAIHPDisplay();
    void hideAllPlayerHPWidgets();
    void hideAllAIHPWidgets();

    // Chakra system functions
    void updatePlayerChakraDisplay();
    void updateAIChakraDisplay();

    // Character state functions
    void resetNarutoState();
    void resetBleachState();
    void startResetTimer();

    // Game end functions
    void checkGameEnd();
    void endGame(bool playerWon);

    // Power-Up System functions
    void checkTurnComplete();
    void giveRandomPowerUps();
    void giveRandomPowerUpsAlternative();
    void giveRandomPowerUpsHighHealChakra();
    void applyInstantPowerUp(bool isPlayer, int type);
    void updatePlayerPowerUpDisplay();
    void updateAIPowerUpDisplay();
    void hideAllPlayerPowerUps();
    void hideAllAIPowerUps();
    void clearPowerUpAfterUse(bool isPlayer);

    // Damage display system
    void showDamageText(int playerDamage, int aiDamage);
    void hideDamageText();
    void updateDamageDisplay();

    // Utility functions
    int generateRandomNumber(int min, int max);
    void gainChakra(int amount);
    void useChakra(int amount);

    // GPIO functions
    void initGPIO();
    void toggleLED();

    // Debug functions
    void debugGameState();

protected:
    // Game state
    int playerHP;
    int aiHP;
    bool menuOpen;
    bool gameEnded;
    bool playerWonGame;

    // Chakra system
    int playerChakraLevel; // 0-100
    int aiChakraLevel; // 0-100 (cho AI)

    // Combat state
    bool playerDefending;
    bool aiDefending;
    bool playerSpecialUsed;
    bool aiSpecialUsed;

    // AI personality (0=Aggressive, 1=Defensive, 2=Random)
    int aiPersonality;

    // ===== TIMER SYSTEM =====
    int resetTimer;
    bool resetTimerActive;
    int autoReturnTimer; // Timer để tự động về main menu sau 6 giây
    bool autoReturnTimerActive;
    static const int RESET_TIME_MS = 1000; // 1 second
    static const int AUTO_RETURN_TIME_MS = 6000; // 6 seconds delay before returning to menu

    // Power-Up System variables
    int turnCounter;
    int playerPowerUpType; // 0=none, 1=damage, 2=heal, 3=chakra, 4=shield
    int aiPowerUpType;
    bool playerPowerUpActive;
    bool aiPowerUpActive;
    bool playerActionDone;
    bool aiActionDone;

    // Damage display system variables
    int damageDisplayTimer;
    bool damageDisplayActive;
    static const int DAMAGE_DISPLAY_TIME_MS = 1500; // 1.5 giây hiển thị damage text

    // Tracking variables for damage display
    int currentDamageAmount;
    bool isPlayerDamage;
    int currentPlayerDamage;
    int currentAIDamage;
    bool playerDamageActive;
    bool aiDamageActive;

    // Animation variables for damage text movement
    int damageTextStartY;
    int damageTextAIStartY;
    static const int DAMAGE_TEXT_MOVE_DISTANCE = 30;

    int calculateSpecialDamage(int baseDamage, bool isDefending, bool hasShield);
    uint32_t randomSeed;
};

#endif // GAMESCREEN_VIEW_HPP
