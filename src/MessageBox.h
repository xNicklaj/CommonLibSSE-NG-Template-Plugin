// Taken from https://github.com/SkyrimScripting/MessageBox/blob/main/include/SkyrimScripting/MessageBox.h
#pragma once

#include <functional>
#include <future>
#include <vector>

namespace SkyrimScripting {

    class SkyrimMessageBox {
        class MessageBoxResultCallback : public RE::IMessageBoxCallback {
            std::function<void(unsigned int)> _callback;

        public:
            ~MessageBoxResultCallback() override {}
            MessageBoxResultCallback(std::function<void(unsigned int)> callback) : _callback(callback) {}
            void Run(std::uint8_t a_button) override {
                _callback(static_cast<unsigned int>(a_button));
            }
        };

    public:
        static void Show(const std::string& bodyText, std::vector<std::string> buttonTextValues,
            std::function<void(unsigned int)> callback) {
            auto* factoryManager = RE::MessageDataFactoryManager::GetSingleton();
            auto* uiStringHolder = RE::InterfaceStrings::GetSingleton();
            auto* factory = factoryManager->GetCreator<RE::MessageBoxData>(
                uiStringHolder->messageBoxData);
            auto* messagebox = factory->Create();
            RE::BSTSmartPointer<RE::IMessageBoxCallback> messageCallback =
                RE::make_smart<MessageBoxResultCallback>(callback);
            messagebox->callback = messageCallback;
            messagebox->bodyText = bodyText;
            for (auto text : buttonTextValues) messagebox->buttonText.push_back(text.c_str());
            auto* msgQueue = RE::UIMessageQueue::GetSingleton();
            if (msgQueue) {
                msgQueue->AddMessage(uiStringHolder->messageBoxMenu, RE::UI_MESSAGE_TYPE::kShow, messagebox);
            }
        }
    };

    void ShowMessageBox(const std::string& bodyText, std::vector<std::string> buttonTextValues,
        std::function<void(unsigned int)> callback) {
        SkyrimMessageBox::Show(bodyText, buttonTextValues, callback);
    }
}