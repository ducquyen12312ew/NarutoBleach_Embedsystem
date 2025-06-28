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
    int calculateSpecialDamage(int baseDamage, bool isDefending, bool hasShield);

    void updatePlayerHPDisplay();
    void updateBossHPDisplay();
    void hideAllPlayerHPWidgets();
    void hideAllBossHPWidgets();

    void updatePlayerChakraDisplay();
    void updateAIChakraDisplay();
    void gainChakra(int amount);
    void useChakra(int amount);

    void resetNarutoState();
    void resetBleachState();
    void startResetTimer();

    void checkGameEnd();
    void endGame(bool playerWon);

    void checkTurnComplete();
    void givePlayerPowerUp();
    void applyInstantPowerUp(bool isPlayer, int type);
    void updatePlayerPowerUpDisplay();
    void updateAIPowerUpDisplay();
    void hideAllPlayerPowerUps();
    void hideAllAIPowerUps();
    void clearPowerUpAfterUse(bool isPlayer);

    void hideAllFireEffects();
    void startAttackFireAnimation();
    void startSpecialFireAnimation();
    void updateFireAnimation();

    int generateRandomNumber(int min, int max);

    void initGPIO();
    void toggleLED();

    void debugGameState();
    void forceBossHPVisible();
    void debugBossHP();
    void activateEasterEgg();

protected:
    int playerHP;
    bool menuOpen;
    bool gameEnded;

    int bossPhase;
    int bossMaxHP;
    int bossCurrentHP;
    bool rageMode;
    int rageModeTimer;

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

    int fireAnimationTimer;
    bool fireAnimationActive;
    int fireAnimationState;
    int fireLoopCounter;
    bool specialFireToggle;
    static const int FIRE_ANIMATION_DURATION = 500;
    static const int FIRE_LOOP_INTERVAL = 300;

    uint32_t randomSeed;

    bool easterEggActivated;
    int easterEggTimer;
    static const int EASTER_EGG_WIN_TIME = 3000;
};

#endif
