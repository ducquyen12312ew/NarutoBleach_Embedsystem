#include <gui/screen1_screen/Screen1View.hpp>
#include "stm32f4xx_hal.h"

Screen1View::Screen1View()
{
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
    initGPIO(); // Khởi tạo GPIO cho LED PG13
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::initGPIO()
{
    // Khởi tạo GPIO cho LED PG13
    __HAL_RCC_GPIOG_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    // Tắt LED ban đầu
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
}

void Screen1View::toggleLED()
{
    // Toggle LED PG13
    HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
}

void Screen1View::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        // Kiểm tra xem có chạm vào naruto không
        // Từ code của bạn: naruto.setXY(8, 78);
        if (naruto.getAbsoluteRect().intersect(evt.getX(), evt.getY()))
        {
            toggleLED(); // Bật/tắt LED khi chạm vào naruto
        }
    }

    // Gọi parent handler
    Screen1ViewBase::handleClickEvent(evt);
}

void Screen1View::handleDragEvent(const DragEvent& evt)
{
    // Gọi parent handler
    Screen1ViewBase::handleDragEvent(evt);
}
