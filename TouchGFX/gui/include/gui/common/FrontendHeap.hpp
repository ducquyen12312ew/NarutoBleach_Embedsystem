#ifndef FRONTENDHEAP_HPP
#define FRONTENDHEAP_HPP

#include <gui_generated/common/FrontendHeapBase.hpp>

class FrontendHeap : public FrontendHeapBase
{
public:
    typedef touchgfx::meta::TypeList<
        touchgfx::meta::Nil,
        touchgfx::meta::Nil
    > UserDefinedViewTypes;

    typedef touchgfx::meta::TypeList<
        touchgfx::meta::Nil,
        touchgfx::meta::Nil
    > UserDefinedPresenterTypes;

    typedef touchgfx::meta::TypeList<
        touchgfx::meta::Nil,
        touchgfx::meta::Nil
    > UserDefinedTransitionTypes;

    typedef touchgfx::meta::select_type_maxsize<UserDefinedViewTypes>::type MaxUserViewType;
    typedef touchgfx::meta::TypeList<
        MaxGeneratedViewType,
        touchgfx::meta::TypeList<MaxUserViewType, touchgfx::meta::Nil>
    > CombinedViewTypes;
    typedef touchgfx::meta::select_type_maxsize<CombinedViewTypes>::type MaxViewType;

    typedef touchgfx::meta::select_type_maxsize<UserDefinedPresenterTypes>::type MaxUserPresenterType;
    typedef touchgfx::meta::TypeList<
        MaxGeneratedPresenterType,
        touchgfx::meta::TypeList<MaxUserPresenterType, touchgfx::meta::Nil>
    > CombinedPresenterTypes;
    typedef touchgfx::meta::select_type_maxsize<CombinedPresenterTypes>::type MaxPresenterType;

    typedef touchgfx::meta::select_type_maxsize<UserDefinedTransitionTypes>::type MaxUserTransitionType;
    typedef touchgfx::meta::TypeList<
        MaxGeneratedTransitionType,
        touchgfx::meta::TypeList<MaxUserTransitionType, touchgfx::meta::Nil>
    > CombinedTransitionTypes;
    typedef touchgfx::meta::select_type_maxsize<CombinedTransitionTypes>::type MaxTransitionType;

    static FrontendHeap& getInstance()
    {
        static FrontendHeap instance;
        return instance;
    }

    touchgfx::Partition<CombinedPresenterTypes, 1> presenters;
    touchgfx::Partition<CombinedViewTypes, 1> views;
    touchgfx::Partition<CombinedTransitionTypes, 1> transitions;
    Model model;
    FrontendApplication app;

private:
    FrontendHeap() : FrontendHeapBase(presenters, views, transitions, app),
                     app(model, *this)
    {
        gotoStartScreen(app);
    }
};

#endif
