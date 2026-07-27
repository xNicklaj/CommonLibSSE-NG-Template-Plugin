#ifndef ACHIEVEMENT_MENU_H
#define ACHIEVEMENT_MENU_H

#include "PCH.h"
#include "RE/Skyrim.h"
#include "RE/G/GFxMovieDef.h"
#include "RE/G/GFxValue.h"
#include "RE/G/GPtr.h"
#include "SKSE/SKSE.h"
#include "Utility.h"

#include "PaddedIMenu.h"

namespace Scaleform {

    class AchievementMenu : public PaddedIMenu, public RE::BSTEventSink<SKSE::ModCallbackEvent>, public RE::BSTEventSink<RE::MenuOpenCloseEvent>, public RE::BSTEventSink<RE::InputEvent*> {
    public:
        static constexpr const char* MENU_PATH = "achievementmenu";
        static constexpr const char* MENU_NAME = "AchievementMenu";

        AchievementMenu();

        static void Register();
        static void Show();
        static void Hide();
        static void UpdateAchievementList();
        static void UpdateAchievementList(std::string data);

        virtual RE::BSEventNotifyControl ProcessEvent(const SKSE::ModCallbackEvent* a_event, RE::BSTEventSource<SKSE::ModCallbackEvent>* a_eventSource);
        virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource);
        virtual RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource);

        static constexpr std::string_view Name();

        static RE::stl::owner<RE::IMenu*> Creator() { return new AchievementMenu(); }
    };

    constexpr std::string_view AchievementMenu::Name() { return AchievementMenu::MENU_NAME; }

    class AchievementMenuInjector : public PaddedIMenu, public RE::BSTEventSink<SKSE::ModCallbackEvent> {
    public:
        static constexpr const char* MENU_PATH = "AchievementMenuInjector";
        static constexpr const char* MENU_NAME = "AchievementMenuInjector";

        static void Inject();

        virtual RE::BSEventNotifyControl ProcessEvent(const SKSE::ModCallbackEvent* a_event, RE::BSTEventSource<SKSE::ModCallbackEvent>* a_eventSource);
    
        static RE::stl::owner<RE::IMenu*> Creator() { return new AchievementMenuInjector(); }

        static void Register();

        static constexpr std::string_view Name();
    };

    constexpr std::string_view AchievementMenuInjector::Name() { return AchievementMenuInjector::MENU_NAME; }

    class CategoryButtonPressHandler : public RE::GFxFunctionHandler {
        RE::GFxValue originalFunc;
    public:
        CategoryButtonPressHandler(RE::GFxValue a_orig) : originalFunc(a_orig) {}
        void Call(Params& a_params) override;
    };

    class InvalidateDataHandler : public RE::GFxFunctionHandler {
        RE::GFxValue originalFunc;
        RE::GFxValue listMc;
    public:
        InvalidateDataHandler(RE::GFxValue a_orig, RE::GFxValue a_listMc) : originalFunc(a_orig), listMc(a_listMc) {}
        void Call(Params& a_params) override;
    };

}

#endif  // ACHIEVEMENT_MENU_H
