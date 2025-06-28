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

    void hideActionButtons();
    void showActionButtons();
    void toggleActionMenu();

    void playerAttack();
    void playerDefend();
    void playerSpecial();

    void performAIAction();
    void aiAttack();
    void aiDefend();
    void aiSpecial();

    int calculateDamage(int baseDamage, bool isDefending, bool hasDoubleAttack, bool hasShield);

    void updatePlayerHPDisplay();
    void updateAIHPDisplay();
    void hideAllPlayerHPWidgets();
    void hideAllAIHPWidgets();

    void updatePlayerChakraDisplay();
    void updateAIChakraDisplay();

    void resetNarutoState();
    void resetBleachState();
    void startResetTimer();

    void checkGameEnd();
    void endGame(bool playerWon);

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

    void showDamageText(int playerDamage, int aiDamage);
    void hideDamageText();
    void updateDamageDisplay();

    int generateRandomNumber(int min, int max);
    void gainChakra(int amount);
    void useChakra(int amount);

    void initGPIO();
    void toggleLED();

    void debugGameState();

protected:
    int playerHP;
    int aiHP;
    bool menuOpen;
    bool gameEnded;
    bool playerWonGame;

    int playerChakraLevel;
    int aiChakraLevel;

    bool playerDefending;
    bool aiDefending;
    bool playerSpecialUsed;
    bool aiSpecialUsed;

    int aiPersonality;

    int resetTimer;
    bool resetTimerActive;
    int autoReturnTimer;
    bool autoReturnTimerActive;
    static const int RESET_TIME_MS = 1000;
    static const int AUTO_RETURN_TIME_MS = 6000;

    int turnCounter;
    int playerPowerUpType;
    int aiPowerUpType;
    bool playerPowerUpActive;
    bool aiPowerUpActive;
    bool playerActionDone;
    bool aiActionDone;

    int damageDisplayTimer;
    bool damageDisplayActive;
    static const int DAMAGE_DISPLAY_TIME_MS = 1500;

    int currentDamageAmount;
    bool isPlayerDamage;
    int currentPlayerDamage;
    int currentAIDamage;
    bool playerDamageActive;
    bool aiDamageActive;

    int damageTextStartY;
    int damageTextAIStartY;
    static const int DAMAGE_TEXT_MOVE_DISTANCE = 30;

    int calculateSpecialDamage(int baseDamage, bool isDefending, bool hasShield);
    uint32_t randomSeed;
};

#endif
