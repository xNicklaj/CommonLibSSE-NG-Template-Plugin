#include "AchievementMenu.h"
#include "Scaleform.h"
#include "Utility.h"
#include "AchievementManager.h"
#include "Interface.h"
#include "SKSE/SKSE.h"
#include "KeyMapping.h"

namespace Scaleform {
    AchievementMenu::AchievementMenu() {
        auto scaleformManager = RE::BSScaleformManager::GetSingleton();
        scaleformManager->LoadMovieEx(this, MENU_PATH, [this](RE::GFxMovieDef* a_def) {
            using StateType = RE::GFxState::StateType;
            fxDelegate.reset(new RE::FxDelegate());
            fxDelegate->RegisterHandler(this);
            a_def->SetState(StateType::kExternalInterface, fxDelegate.get());
            fxDelegate->Release();
            auto logger = new Logger<AchievementMenu>();
            a_def->SetState(StateType::kLog, logger);
            logger->Release();
        });

        inputContext = Context::kMenuMode;
        depthPriority = 3;

        auto& view = this->uiMovie;
        //view->SetMouseCursorCount(0);
        Internal::Interface::FunctionManager::AttachSKSEFunctions(view);
        
        //RE::UIBlurManager::GetSingleton()->blurCount = 0;

        
        bool isSkyrimSoulsLoaded = GetModuleHandleA("SkyrimSoulsRE.dll") != NULL;
        if (isSkyrimSoulsLoaded) {
            // Skyrim Souls COMPAT
            menuFlags.set(RE::UI_MENU_FLAGS::kDisablePauseMenu, RE::UI_MENU_FLAGS::kModal,
                          RE::UI_MENU_FLAGS::kUsesMenuContext, RE::UI_MENU_FLAGS::kTopmostRenderedMenu);
        } else {
            menuFlags.set(RE::UI_MENU_FLAGS::kPausesGame, RE::UI_MENU_FLAGS::kDisablePauseMenu, RE::UI_MENU_FLAGS::kModal,
                          RE::UI_MENU_FLAGS::kUsesMenuContext, RE::UI_MENU_FLAGS::kTopmostRenderedMenu,
                          RE::UI_MENU_FLAGS::kUsesMovementToDirection);
        }

        if (!RE::BSInputDeviceManager::GetSingleton()->IsGamepadEnabled()) {
            menuFlags |= RE::UI_MENU_FLAGS::kUsesCursor;
        }
        
    }

    void AchievementMenu::Register() {
        auto ui = RE::UI::GetSingleton();
        static AchievementMenu singleton;
        auto eventSource = SKSE::GetModCallbackEventSource();
        if (!eventSource) {
            logger::error("EventSource not found!");
            return;
        }
        if (ui) {
            ui->Register(AchievementMenu::MENU_NAME, Creator);
            logger::debug("Registered {}", AchievementMenu::MENU_NAME);
            eventSource->AddEventSink(&singleton);
            ui->AddEventSink<RE::MenuOpenCloseEvent>(&singleton);
            RE::BSInputDeviceManager::GetSingleton()->AddEventSink<RE::InputEvent*>(&singleton);
        }
    }

    void AchievementMenu::Show() {
        auto uiMessageQueue = RE::UIMessageQueue::GetSingleton();
        if (uiMessageQueue) {
            uiMessageQueue->AddMessage(AchievementMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
            RE::UIBlurManager::GetSingleton()->IncrementBlurCount();
        }
    }

    void AchievementMenu::Hide() {
        auto uiMessageQueue = RE::UIMessageQueue::GetSingleton();
        if (uiMessageQueue) {
            uiMessageQueue->AddMessage(AchievementMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
            RE::UIBlurManager::GetSingleton()->DecrementBlurCount();
            //RE::UIBlurManager::GetSingleton()->blurCount = 0;
        }
    }

    void AchievementMenu::UpdateAchievementList(std::string data) {
        auto ui = RE::UI::GetSingleton();
        RE::GFxValue widget;
        // Check if menu is found
        if (ui->GetMenu(AchievementMenu::MENU_NAME) == nullptr) {
            logger::debug("{} not found.", AchievementMenu::MENU_NAME);
            return;
        }
        if (ui->GetMenu(AchievementMenu::MENU_NAME)->uiMovie->GetVariable(&widget, "_root.MenuFader_mc.Menu_mc")) {
            std::array<RE::GFxValue, 1> args;
            json test = AchievementManager::GetSingleton()->ToJson();
            args[0] = data;
            widget.Invoke("setData", nullptr, args.data(), args.size());
            args[0] = 1;
            widget.Invoke("setDefaultIndex", nullptr, args.data(), args.size());
        }
    }

    RE::BSEventNotifyControl AchievementMenu::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) {
        if (a_event->opening) {
            if (a_event->menuName == Scaleform::AchievementMenu::MENU_NAME) {
                auto* ui = RE::UI::GetSingleton();
                if (ui != nullptr && ui->GetMenu(Scaleform::AchievementMenu::MENU_NAME) != nullptr) {
                    AchievementMenu::UpdateAchievementList(AchievementManager::GetSingleton()->cache);
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    };

    RE::BSEventNotifyControl AchievementMenu::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) {
        auto* ui = RE::UI::GetSingleton();
        if (!a_event || !(*a_event) || !RE::Main::GetSingleton()->GetRuntimeData().gameActive) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if ((*a_event)->eventType == RE::INPUT_EVENT_TYPE::kButton) {
            if (ui && ui->IsMenuOpen(AchievementMenu::Name())) {
                auto a_buttonEvent = (*a_event)->AsButtonEvent();
                std::string keyName = GetKeyNameFromScanCode(a_buttonEvent->GetIDCode());
                if ((*a_event)->GetDevice() == RE::INPUT_DEVICE::kGamepad) {
                    if (a_buttonEvent->GetIDCode() == 8192 || a_buttonEvent->GetIDCode() == 16 || keyName == "B" || keyName == "START") {
                        AchievementMenu::Hide();
                    }
                }
                else if ((*a_event)->GetDevice() == RE::INPUT_DEVICE::kKeyboard) {
                    if (keyName == "Escape" || keyName == "Tab") {
                        AchievementMenu::Hide();
                    }
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl AchievementMenu::ProcessEvent(const SKSE::ModCallbackEvent* a_event, RE::BSTEventSource<SKSE::ModCallbackEvent>*) {
        //RE::UI* ui = RE::UI::GetSingleton();
        RE::GFxValue menu;
        //logger::debug("Event: {}", a_event->eventName.data());
        if (a_event->eventName == "AchievementMenu_Init") {
            logger::debug("Init");
        }
        else if (a_event->eventName == "AchievementMenu_Close") {
            AchievementMenu::Hide();
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    static int s_loadGameIndex = 0;

    void CategoryButtonPressHandler::Call(Params& a_params) {
        if (a_params.argCount > 0) {
            RE::GFxValue event = a_params.args[0];
            if (event.IsObject()) {
                RE::GFxValue entry;
                // VR sometimes uses 'item', SE uses 'entry'
                if ((event.GetMember("entry", &entry) && entry.IsObject()) || 
                    (event.GetMember("item", &entry) && entry.IsObject())) {
                    
                    RE::GFxValue isAchievements;
                    if (entry.GetMember("isAchievements", &isAchievements) && isAchievements.IsBool() && isAchievements.GetBool()) {
                        SKSE::ModCallbackEvent modEvent{ "AchievementsMenu_Open", RE::BSFixedString(), 0.0f, nullptr };
                        SKSE::GetModCallbackEventSource()->SendEvent(&modEvent);
                        return; // Do NOT call original function for our custom button
                    }
                }
                
                RE::GFxValue indexVal;
                if (!REL::Module::IsVR() && event.GetMember("index", &indexVal) && indexVal.IsNumber()) {
                    if (indexVal.GetNumber() > s_loadGameIndex) {
                        event.SetMember("index", indexVal.GetNumber() - 1);
                    }
                }
            }
        }
        
        // Preserve 'this' context by calling ActionScript 'call' method
        std::vector<RE::GFxValue> callArgs;
        callArgs.push_back(*a_params.thisPtr);
        for (uint32_t i = 0; i < a_params.argCount; ++i) {
            callArgs.push_back(a_params.args[i]);
        }
        originalFunc.Invoke("call", nullptr, callArgs.data(), callArgs.size());
    }

    void InvalidateDataHandler::Call(Params& a_params) {
        RE::GFxValue entryList;
        if (listMc.GetMember("entryList", &entryList) && entryList.IsArray()) {
            bool hasButton = false;
            uint32_t len = entryList.GetArraySize();
            
            for (uint32_t i = 0; i < len; ++i) {
                RE::GFxValue entry;
                entryList.GetElement(i, &entry);
                if (entry.IsObject()) {
                    RE::GFxValue isAchievements;
                    if (entry.GetMember("isAchievements", &isAchievements) && isAchievements.IsBool() && isAchievements.GetBool()) {
                        hasButton = true;
                        break;
                    }
                    
                    RE::GFxValue text;
                    if (entry.GetMember("text", &text) && text.IsString()) {
                        if (std::string(text.GetString()) == "$LOAD") {
                            s_loadGameIndex = i + 1;
                        }
                    }
                }
            }
            
            if (!hasButton) {
                RE::GFxValue newButton;
                a_params.movie->CreateObject(&newButton);
                newButton.SetMember("text", "$ACH_MENULBL");
                newButton.SetMember("isAchievements", true);
                
                RE::GFxValue spliceArgs[3];
                spliceArgs[0] = static_cast<double>(s_loadGameIndex); // Index to insert
                spliceArgs[1] = 0.0; // Number of items to delete
                spliceArgs[2] = newButton;
                entryList.Invoke("splice", nullptr, spliceArgs, 3);
            }
        }
        
        std::vector<RE::GFxValue> callArgs;
        callArgs.push_back(*a_params.thisPtr);
        for (uint32_t i = 0; i < a_params.argCount; ++i) {
            callArgs.push_back(a_params.args[i]);
        }
        originalFunc.Invoke("call", nullptr, callArgs.data(), callArgs.size());
    }

    void AchievementMenuInjector::Inject() {
        auto ui = RE::UI::GetSingleton();
        auto journalMenu = ui->GetMenu("Journal Menu");
        if (!journalMenu || !journalMenu->uiMovie) return;

        RE::GFxValue isAppended;
        if (journalMenu->uiMovie->GetVariable(&isAppended, "_root.isAchievementsMenuInjectorAppended") && isAppended.GetBool()) {
            return; // Already injected
        }

        RE::GFxValue systemPage;
        if (!journalMenu->uiMovie->GetVariable(&systemPage, "_root.QuestJournalFader.Menu_mc.SystemFader.Page_mc")) {
            return;
        }

        RE::GFxValue categoryList, listMc;
        if (!systemPage.GetMember("CategoryList_mc", &categoryList) || !categoryList.GetMember("List_mc", &listMc)) {
            return;
        }

        journalMenu->uiMovie->SetVariable("_root.isAchievementsMenuInjectorAppended", RE::GFxValue(true));

        RE::GFxValue originalOnCategoryButtonPress;
        if (systemPage.GetMember("onCategoryButtonPress", &originalOnCategoryButtonPress) && !originalOnCategoryButtonPress.IsUndefined()) {
            RE::GPtr<CategoryButtonPressHandler> pressHandler{new CategoryButtonPressHandler(originalOnCategoryButtonPress)};
            RE::GFxValue newOnCategoryButtonPress;
            journalMenu->uiMovie->CreateFunction(&newOnCategoryButtonPress, pressHandler.get());
            systemPage.SetMember("onCategoryButtonPress", newOnCategoryButtonPress);
        }

        RE::GFxValue originalInvalidateData;
        if (listMc.GetMember("InvalidateData", &originalInvalidateData) && !originalInvalidateData.IsUndefined()) {
            RE::GPtr<InvalidateDataHandler> invalidateHandler{new InvalidateDataHandler(originalInvalidateData, listMc)};
            RE::GFxValue newInvalidateData;
            journalMenu->uiMovie->CreateFunction(&newInvalidateData, invalidateHandler.get());
            listMc.SetMember("InvalidateData", newInvalidateData);
            
            // Wait for C++ engine to populate the array and call InvalidateData naturally!
        }
    }

    RE::BSEventNotifyControl AchievementMenuInjector::ProcessEvent(const SKSE::ModCallbackEvent* a_event, RE::BSTEventSource<SKSE::ModCallbackEvent>*) {
        RE::UI* ui = RE::UI::GetSingleton();
        RE::GFxValue menu;
        if (a_event->eventName == "AchievementsMenu_Open") {
            if (ui->GetMenu("HUD Menu")->uiMovie->GetVariable(&menu, "_global.skse")) {
                std::array<RE::GFxValue, 1> args;
                args[0] = "Journal Menu";
                menu.Invoke("CloseMenu", args);
            }
            AchievementMenu::Show();
            
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    void AchievementMenuInjector::Register() {
        static AchievementMenuInjector singleton;

        auto eventSource = SKSE::GetModCallbackEventSource();

        if (!eventSource) {
            logger::error("EventSource not found!");
            return;
        }
        eventSource->AddEventSink(&singleton);
        logger::info("Register {}", typeid(singleton).name());
        return;
	}
}
