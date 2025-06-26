#include <gui/sponge_screen/SpongeView.hpp>
#include <touchgfx/Color.hpp>
#include "stm32f4xx_hal.h"

SpongeView::SpongeView()
{
    resetTimer = 0;
    resetTimerActive = false;
    menuOpen = false;
}

void SpongeView::setupScreen()
{
    SpongeViewBase::setupScreen();
    initGPIO();

    // Initialize training mode
    menuOpen = false;

    // Reset timer system
    resetTimer = 0;
    resetTimerActive = false;

    // Ensure sponge is in normal state when starting
    resetSpongeState();

    // Hide action buttons initially
    hideActionButtons();
}

void SpongeView::tearDownScreen()
{
    SpongeViewBase::tearDownScreen();
}

void SpongeView::initGPIO()
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

void SpongeView::toggleLED()
{
    HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
}

void SpongeView::hideActionButtons()
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

void SpongeView::showActionButtons()
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

void SpongeView::toggleActionMenu()
{
    if (menuOpen) {
        hideActionButtons();
    } else {
        showActionButtons();
    }
    toggleLED();
}

void SpongeView::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED) {

        // Handle arrow buttons to show/hide menu
        if (btn_arrow1.getAbsoluteRect().intersect(evt.getX(), evt.getY())) {
            toggleActionMenu();
            return;
        }

        if (btn_arrow2.getAbsoluteRect().intersect(evt.getX(), evt.getY())) {
            toggleActionMenu();
            return;
        }

        // Handle action buttons when menu is open
        if (menuOpen) {
            if (btn_attack.getAbsoluteRect().intersect(evt.getX(), evt.getY())) {
                performAttack();
                return;
            }
            else if (btn_def.getAbsoluteRect().intersect(evt.getX(), evt.getY())) {
                performDefend();
                return;
            }
            else if (btn_spe.getAbsoluteRect().intersect(evt.getX(), evt.getY())) {
                performSpecial();
                return;
            }
        }
    }

    SpongeViewBase::handleClickEvent(evt);
}

void SpongeView::handleTickEvent()
{
    // Handle auto reset timer for returning to normal state
    if (resetTimerActive) {
        resetTimer += 16; // TouchGFX tick ~16ms
        if (resetTimer >= RESET_TIME_MS) {
            resetSpongeState();
            resetTimerActive = false;
            resetTimer = 0;
        }
    }

    SpongeViewBase::handleTickEvent();
}

void SpongeView::performAttack()
{
    resetSpongeState();

    // Show attack animation
    naruto.setVisible(false);
    naruto_atk.setVisible(true);
    naruto_atk.invalidate();
    naruto.invalidate();

    // LED feedback
    toggleLED();

    // Start timer to return to normal state
    startResetTimer();
}

void SpongeView::performDefend()
{
    resetSpongeState();

    // Show defend animation
    naruto.setVisible(false);
    naruto_def.setVisible(true);
    naruto_def.invalidate();
    naruto.invalidate();

    // LED feedback
    toggleLED();

    // Start timer to return to normal state
    startResetTimer();
}

void SpongeView::performSpecial()
{
    resetSpongeState();

    // Show special animation - BOTH naruto_spe AND naruto_spe_1 together
    naruto.setVisible(false);
    naruto_spe.setVisible(true);
    naruto_spe_1.setVisible(true);  // Show both special sprites simultaneously

    naruto_spe.invalidate();
    naruto_spe_1.invalidate();
    naruto.invalidate();

    // Special LED pattern for special move
    for (int i = 0; i < 3; i++) {
        toggleLED();
    }

    // Start timer to return to normal state
    startResetTimer();
}

void SpongeView::resetSpongeState()
{
    // Hide all special state sprites
    naruto_atk.setVisible(false);
    naruto_def.setVisible(false);
    naruto_spe.setVisible(false);
    naruto_spe_1.setVisible(false);  // Hide second special sprite too

    // Show normal sponge sprite
    naruto.setVisible(true);

    // Invalidate all sprites
    naruto_atk.invalidate();
    naruto_def.invalidate();
    naruto_spe.invalidate();
    naruto_spe_1.invalidate();
    naruto.invalidate();
}

void SpongeView::startResetTimer()
{
    resetTimer = 0;
    resetTimerActive = true;
}
