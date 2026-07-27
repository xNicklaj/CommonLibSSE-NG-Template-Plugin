#include "log.h"
#include "CommonFunctions.h"
#include "Singleton.h"
#include "Papyrus.h"
#include "KeyMapping.h"
#include <string>
#include <cctype>

namespace logger = SKSE::log;

std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();  // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

/* Usage: https://stackoverflow.com/a/2815918 */
std::string NumToString(long num) {
    std::ostringstream oss;
    oss << num;
    return oss.str();
};

uint16_t CheckQuestStage(std::string FormID, std::string baseEsp) {
    // TODO dynamic esp
    // Get the quest form using the Editor Form ID
    auto* target = GetForm(FormID, baseEsp);
    if (!target) {
        logger::error("Could not find quest with FormID {} in {}.", FormID, baseEsp);
        return 0;
    }
    const auto quest = RE::TESForm::LookupByID<RE::TESQuest>(target->formID);

    // Get the current stage of the quest
    auto currentStage = quest->GetCurrentStageID();
    return currentStage;
}

template <typename T>
T GetFormOfType(std::string FormID, std::string baseEsp = "Skyrim.esm") {
    // Get the TESDataHandler instance
    auto dataHandler = RE::TESDataHandler::GetSingleton();
    if (!dataHandler) {
        logger::error("Failed to get TESDataHandler instance.");
        return nullptr;
    }

    RE::FormID actualFormID;
    std::istringstream converter(FormID);
    converter >> std::hex >> actualFormID;
    const auto form = RE::TESDataHandler::GetSingleton()->LookupForm<T>(actualFormID, baseEsp);
    if (!form) {
        //logger::error("Failed to find form with Form ID {} and IntFormID {} in plugin {}", FormID, actualFormID, baseEsp);
        return nullptr;
    }

    return form;
}

RE::TESForm* GetForm(std::string FormID, std::string baseEsp = "Skyrim.esm") {
	// Get the TESDataHandler instance
	auto dataHandler = RE::TESDataHandler::GetSingleton();
	if (!dataHandler) {
		logger::error("Failed to get TESDataHandler instance.");
		return nullptr;
	}

	// Get the quest form using the Editor Form ID
    RE::FormID actualFormID;
    std::istringstream converter(FormID);
    converter >> std::hex >> actualFormID;
    const auto form = RE::TESDataHandler::GetSingleton()->LookupForm(actualFormID, baseEsp);
	if (!form) {
		//logger::error("Failed to find form with Form ID {} and IntFormID {} in plugin {}", FormID, actualFormID, baseEsp);
		return nullptr;
	}

	return form;
}

std::string FormIDToString(RE::FormID formID) {
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << formID;
    return ss.str();
}

RE::ActorValue StringToActorValue(std::string value) {
    std::string val_lower = value;
    std::transform(val_lower.begin(), val_lower.end(), val_lower.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    
    if (val_lower == "alchemy") return RE::ActorValue::kAlchemy;
    else if (val_lower == "alteration") return RE::ActorValue::kAlteration;
    else if (val_lower == "marksman" || val_lower == "archery") return RE::ActorValue::kArchery;
    else if (val_lower == "block") return RE::ActorValue::kBlock;
    else if (val_lower == "conjuration") return RE::ActorValue::kConjuration;
    else if (val_lower == "destruction") return RE::ActorValue::kDestruction;
    else if (val_lower == "enchanting") return RE::ActorValue::kEnchanting;
    else if (val_lower == "heavyarmor") return RE::ActorValue::kHeavyArmor;
    else if (val_lower == "illusion") return RE::ActorValue::kIllusion;
    else if (val_lower == "lightarmor") return RE::ActorValue::kLightArmor;
    else if (val_lower == "lockpicking") return RE::ActorValue::kLockpicking;
    else if (val_lower == "onehanded") return RE::ActorValue::kOneHanded;
    else if (val_lower == "pickpocket") return RE::ActorValue::kPickpocket;
    else if (val_lower == "restoration") return RE::ActorValue::kRestoration;
    else if (val_lower == "smithing") return RE::ActorValue::kSmithing;
    else if (val_lower == "sneak") return RE::ActorValue::kSneak;
    else if (val_lower == "speechcraft" || val_lower == "speech") return RE::ActorValue::kSpeech;
    else if (val_lower == "twohanded") return RE::ActorValue::kTwoHanded;
    return RE::ActorValue::kNone;
}
void ReadJson(const std::string& filePath, json* jsonData) {
    // Open the JSON file
    std::ifstream file(filePath);
    if (!file.is_open()) {
        logger::error("Could not open the file {}!", filePath);
        return;
    }

    // Parse the JSON file
    file >> *jsonData;

    // Close the file
    file.close();
    return;
}

std::string GetPlayerName() {
    RE::PlayerCharacter* pc = RE::PlayerCharacter::GetSingleton();
    if (pc) {
        return pc->GetName();
    }
    return "";
}

namespace PopupQueue {
    typedef struct {
        std::string name;
        std::string description;
    } ShowEntryPayload;
}

float GetGlobalVariableValue(RE::FormID formid) {
    auto dataHandler = RE::TESDataHandler::GetSingleton();
    if (!dataHandler) {
        logger::warn("TESDataHandler not found.");
        return 0.0f;
    }

    for (auto& global : dataHandler->GetFormArray<RE::TESGlobal>()) {
        if (global->GetFormID() == formid) {
            return global->value;
        }
    }

    logger::warn("Global variable with formid %s not found.", formid);
    return 0.0f;
}

bool isHex(const std::string& str) {
    // Check if the string is empty
    if (str.empty()) {
        return false;
    }

    // Iterate through each character in the string
    for (char c : str) {
        // Check if the character is not a digit and not a valid hex letter
        if (!std::isdigit(c) && !((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) {
            return false;
        }
    }

    return true;
}

std::string StripExtension(const std::string& filename) {
    size_t pos = filename.find_last_of('.');
    if (pos != std::string::npos) {
        return filename.substr(0, pos);
    }
    return filename; // Return the original string if no dot is found
}

std::string utf16to8(const std::string path) {
    std::ifstream fin(path, std::ios::binary);
    fin.seekg(0, std::ios::end);
    size_t size = (size_t) fin.tellg();

    //skip BOM
    fin.seekg(2, std::ios::beg);
    size -= 2;

    std::u16string u16((size / 2) + 1, '\0');
    fin.read((char*) &u16[0], size);

    std::string utf8 = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(u16);
    return utf8;
}