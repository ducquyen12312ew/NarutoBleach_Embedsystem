/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/rewardscreen_screen/RewardScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>

RewardScreenViewBase::RewardScreenViewBase() :
    flexButtonCallback(this, &RewardScreenViewBase::flexButtonCallbackHandler)
{
    __background.setPosition(0, 0, 240, 320);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    background.setBitmap(touchgfx::Bitmap(BITMAP_REWARD_ID));
    background.setPosition(0, -6, 240, 334);
    background.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);
    add(background);

    home.setBitmap(touchgfx::Bitmap(BITMAP_HOME_ID));
    home.setPosition(187, 278, 59, 42);
    home.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);
    add(home);

    REWARD.setBoxWithBorderPosition(0, 0, 44, 47);
    REWARD.setBorderSize(5);
    REWARD.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(0, 102, 153), touchgfx::Color::getColorFromRGB(0, 153, 204), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(51, 102, 153));
    REWARD.setAlpha(10);
    REWARD.setAction(flexButtonCallback);
    REWARD.setPosition(194, 273, 44, 47);
    add(REWARD);

    sword.setBitmap(touchgfx::Bitmap(BITMAP_SWORD_ID));
    sword.setPosition(25, 30, 61, 78);
    sword.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);
    add(sword);
}

RewardScreenViewBase::~RewardScreenViewBase()
{

}

void RewardScreenViewBase::setupScreen()
{

}

void RewardScreenViewBase::flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (&src == &REWARD)
    {
        //BackHome
        //When REWARD clicked change screen to SakuraMainScreen
        //Go to SakuraMainScreen with no screen transition
        application().gotoSakuraMainScreenScreenNoTransition();
    }
}
