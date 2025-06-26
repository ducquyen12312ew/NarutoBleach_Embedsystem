#ifndef CHARACTER1VIEW_HPP
#define CHARACTER1VIEW_HPP

#include <gui_generated/character1_screen/Character1ViewBase.hpp>
#include <gui/character1_screen/Character1Presenter.hpp>

class Character1View : public Character1ViewBase
{
public:
    Character1View();
    virtual ~Character1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // CHARACTER1VIEW_HPP
