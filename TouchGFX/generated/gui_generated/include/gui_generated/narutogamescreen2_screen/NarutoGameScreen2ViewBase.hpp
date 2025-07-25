/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef NARUTOGAMESCREEN2VIEWBASE_HPP
#define NARUTOGAMESCREEN2VIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/narutogamescreen2_screen/NarutoGameScreen2Presenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/ScalableImage.hpp>

class NarutoGameScreen2ViewBase : public touchgfx::View<NarutoGameScreen2Presenter>
{
public:
    NarutoGameScreen2ViewBase();
    virtual ~NarutoGameScreen2ViewBase();
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::ScalableImage san_Dau;
    touchgfx::ScalableImage BITMAP_HP_100_ID;
    touchgfx::ScalableImage BITMAP_HP_10_ID;
    touchgfx::ScalableImage BITMAP_HP_0_ID;
    touchgfx::ScalableImage BITMAP_HP_20_ID;
    touchgfx::ScalableImage BITMAP_HP_30_ID;
    touchgfx::ScalableImage BITMAP_HP_40_ID;
    touchgfx::ScalableImage BITMAP_HP_50_ID;
    touchgfx::ScalableImage BITMAP_HP_60_ID;
    touchgfx::ScalableImage BITMAP_HP_70_ID;
    touchgfx::ScalableImage BITMAP_HP_80_ID;
    touchgfx::ScalableImage BITMAP_HP_90_ID;
    touchgfx::ScalableImage BITMAP_HP_130_ID_1;
    touchgfx::ScalableImage BITMAP_HP_120_ID_1;
    touchgfx::ScalableImage BITMAP_HP_110_ID_1;
    touchgfx::ScalableImage BITMAP_HP_100_ID_1;
    touchgfx::ScalableImage BITMAP_HP_90_ID_1;
    touchgfx::ScalableImage BITMAP_HP_80_ID_1;
    touchgfx::ScalableImage BITMAP_HP_70_ID_1;
    touchgfx::ScalableImage BITMAP_HP_60_ID_1;
    touchgfx::ScalableImage BITMAP_HP_50_ID_1;
    touchgfx::ScalableImage BITMAP_HP_40_ID_1;
    touchgfx::ScalableImage BITMAP_HP_30_ID_1;
    touchgfx::ScalableImage BITMAP_HP_20_ID_1;
    touchgfx::ScalableImage BITMAP_HP_10_ID_1;
    touchgfx::ScalableImage BITMAP_HP_0_ID_1;
    touchgfx::ScalableImage fire1;
    touchgfx::ScalableImage fire2;
    touchgfx::ScalableImage bleach_lose;
    touchgfx::ScalableImage bleach_win;
    touchgfx::ScalableImage bleach_spe_fire;
    touchgfx::ScalableImage bleach_spe;
    touchgfx::ScalableImage bleach_def;
    touchgfx::ScalableImage bleach_atk;
    touchgfx::ScalableImage bleach;
    touchgfx::ScalableImage naruto;
    touchgfx::ScalableImage naruto_win;
    touchgfx::ScalableImage naruto_lose;
    touchgfx::ScalableImage naruto_spe;
    touchgfx::ScalableImage naruto_def;
    touchgfx::ScalableImage naruto_atk;
    touchgfx::ScalableImage btn_attack;
    touchgfx::ScalableImage btn_def;
    touchgfx::ScalableImage btn_spe;
    touchgfx::ScalableImage btn_arrow2;
    touchgfx::ScalableImage btn_arrow1;
    touchgfx::Box chakra_box1;
    touchgfx::Box chakra_box2;
    touchgfx::ScalableImage gameover;
    touchgfx::ScalableImage win;
    touchgfx::ScalableImage x2dame_2;
    touchgfx::ScalableImage x2dame_1;
    touchgfx::ScalableImage healthplus_1;
    touchgfx::ScalableImage chakragain_1;
    touchgfx::ScalableImage sheild_1;
    touchgfx::ScalableImage sheild_2;
    touchgfx::ScalableImage chakragain_2;
    touchgfx::ScalableImage healthplus_2;

private:

};

#endif // NARUTOGAMESCREEN2VIEWBASE_HPP
