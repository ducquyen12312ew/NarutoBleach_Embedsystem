#ifndef CHARACTER_FINALVIEW_HPP
#define CHARACTER_FINALVIEW_HPP

#include <gui_generated/character_final_screen/Character_FinalViewBase.hpp>
#include <gui/character_final_screen/Character_FinalPresenter.hpp>

class Character_FinalView : public Character_FinalViewBase
{
public:
    Character_FinalView();
    virtual ~Character_FinalView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // CHARACTER_FINALVIEW_HPP
