/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef CHARACTERVIEWBASE_HPP
#define CHARACTERVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/character_screen/CharacterPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/ScalableImage.hpp>
#include <touchgfx/containers/buttons/Buttons.hpp>

class CharacterViewBase : public touchgfx::View<CharacterPresenter>
{
public:
    CharacterViewBase();
    virtual ~CharacterViewBase();
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::ScalableImage background;
    touchgfx::ScalableImage naruto;
    touchgfx::ScalableImage sponge;
    touchgfx::ScalableImage sakura;
    touchgfx::ScalableImage sakura_lock;
    touchgfx::ScalableImage sponge_lock;
    touchgfx::ScalableImage rightarrow;
    touchgfx::ScalableImage leftarrow;
    touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger >  MAINMENU_BUT;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<CharacterViewBase, const touchgfx::AbstractButtonContainer&> flexButtonCallback;

    /*
     * Callback Handler Declarations
     */
    void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);

};

#endif // CHARACTERVIEWBASE_HPP
