/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/character_final_screen/Character_FinalViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>

Character_FinalViewBase::Character_FinalViewBase() :
    flexButtonCallback(this, &Character_FinalViewBase::flexButtonCallbackHandler)
{
    __background.setPosition(0, 0, 240, 320);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    background.setBitmap(touchgfx::Bitmap(BITMAP_PICK_ID));
    background.setPosition(0, 0, 240, 320);
    background.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);
    add(background);

    naruto.setBitmap(touchgfx::Bitmap(BITMAP_NAGUTO_ID));
    naruto.setPosition(62, 76, 110, 160);
    naruto.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);
    naruto.setVisible(false);
    add(naruto);

    sponge.setBitmap(touchgfx::Bitmap(BITMAP_SPONGE_ID));
    sponge.setPosition(57, 65, 132, 185);
    sponge.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);
    add(sponge);

    sakura.setBitmap(touchgfx::Bitmap(BITMAP_SAKURA_ID));
    sakura.setPosition(62, 67, 116, 178);
    sakura.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);
    sakura.setVisible(false);
    add(sakura);

    rightarrow.setBitmap(touchgfx::Bitmap(BITMAP_RIGHTARROW_ID));
    rightarrow.setPosition(197, 135, 42, 54);
    rightarrow.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);
    add(rightarrow);

    leftarrow.setBitmap(touchgfx::Bitmap(BITMAP_LEFTARROW_ID));
    leftarrow.setPosition(0, 135, 42, 54);
    leftarrow.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);
    add(leftarrow);

    spongebutton.setBoxWithBorderPosition(0, 0, 102, 160);
    spongebutton.setBorderSize(0);
    spongebutton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(255, 255, 255), touchgfx::Color::getColorFromRGB(255, 255, 255), touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(51, 102, 153));
    spongebutton.setAlpha(6);
    spongebutton.setAction(flexButtonCallback);
    spongebutton.setPosition(70, 76, 102, 160);
    add(spongebutton);
}

Character_FinalViewBase::~Character_FinalViewBase()
{

}

void Character_FinalViewBase::setupScreen()
{

}

void Character_FinalViewBase::flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (&src == &spongebutton)
    {
        //Sponge
        //When spongebutton clicked change screen to Sponge
        //Go to Sponge with no screen transition
        application().gotoSpongeScreenNoTransition();
    }
}
