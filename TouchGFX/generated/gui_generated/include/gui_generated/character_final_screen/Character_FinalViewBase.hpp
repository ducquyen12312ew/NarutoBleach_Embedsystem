/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef CHARACTER_FINALVIEWBASE_HPP
#define CHARACTER_FINALVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/character_final_screen/Character_FinalPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/ScalableImage.hpp>
#include <touchgfx/containers/buttons/Buttons.hpp>

class Character_FinalViewBase : public touchgfx::View<Character_FinalPresenter>
{
public:
    Character_FinalViewBase();
    virtual ~Character_FinalViewBase();
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
    touchgfx::ScalableImage rightarrow;
    touchgfx::ScalableImage leftarrow;
    touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger >  spongebutton;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<Character_FinalViewBase, const touchgfx::AbstractButtonContainer&> flexButtonCallback;

    /*
     * Callback Handler Declarations
     */
    void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);

};

#endif // CHARACTER_FINALVIEWBASE_HPP
