#include "LocationDiscoveryCondition.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);

// --- Exergist Code ---
// Retrieve all map markers in the game
RE::BSTArray<RE::ObjectRefHandle>* GetPlayerMapMarkers() {
    RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
    std::uint32_t offset = 0;
    if (REL::Module::IsAE() && REL::Module::get().version().patch() >= 629)
        offset = 0x500;
    else if (REL::Module::IsSE() || (REL::Module::IsAE() && REL::Module::get().version().patch() < 629))
        offset = 0x4F8;
    else
        offset = 0xAE8;
    return reinterpret_cast<RE::BSTArray<RE::ObjectRefHandle>*>((uintptr_t) player + offset);
}

// Check if targetLocation or formID is known (map marker visible and enabled)
bool CheckKnownLocation(std::string targetLocation, std::string targetFormID, std::string plugin) {
    auto* playerMapMarkers = GetPlayerMapMarkers();
    for (auto playerMapMarker : *playerMapMarkers) {
        const auto refr = playerMapMarker.get().get();
        if (refr->IsDisabled() == false) { // Check if map marker is NOT disabled
            const auto marker = refr ? refr->extraList.GetByType<RE::ExtraMapMarker>() : nullptr;
            if (marker && marker->mapData) {
                if (marker->mapData->flags.any(RE::MapMarkerData::Flag::kCanTravelTo) == true) { // Check if map marker is visible
                    if (targetFormID != "") {
                        RE::TESObjectREFR* targetRefr = static_cast<RE::TESObjectREFR*>(GetForm(targetFormID, plugin));
                        if (targetRefr && targetRefr->formID == refr->formID) {
                            return true;
                        }
                    } else {
                        auto markerName = marker->mapData->locationName.GetFullName();
                        if (markerName == targetLocation) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

RE::TESObjectREFR* FindMapMarkerByData(RE::MapMarkerData* a_data) {
    auto* playerMapMarkers = GetPlayerMapMarkers();
    for (auto playerMapMarker : *playerMapMarkers) {
        const auto refr = playerMapMarker.get().get();
        if (refr) {
            const auto marker = refr->extraList.GetByType<RE::ExtraMapMarker>();
            if (marker && marker->mapData == a_data) {
                return refr;
            }
        }
    }
    return nullptr;
}
// --- Exergist Code ---

LocationDiscoveryCondition::LocationDiscoveryCondition() : Condition(ConditionType::LocationDiscovery) {}
void LocationDiscoveryCondition::OnDataLoaded(void) {
    try {
        if (this->formID != "") this->cachedRef = static_cast<RE::TESObjectREFR*>(GetForm(this->formID, this->plugin));
        if (CheckKnownLocation(this->locationName, this->formID, this->plugin)) {
            logger::info("Player met condition found {} in {}.", this->formID != "" ? this->formID : this->locationName, this->worldspaceID);
            this->UnlockNotify();
            
        };
    } catch (const std::exception&) {
        logger::error("An error occurred in OnDataLoaded of LocationDiscoveryCondition, with parameters ({}, {})", this->locationName, this->worldspaceID);
    }
    
	ConditionManager::GetSingleton()->RegisterLocationDiscoveryListener(this);
}
void LocationDiscoveryCondition::Localize(std::string path) {
    if(this->locationName != "" && this->locationName[0] == '$')
		this->locationName = LocalizationManager::GetSingleton()->GetLocalizedText(path, LocalizationManager::GetSingleton()->CurrentLocale(), this->locationName);
}
void LocationDiscoveryCondition::EnableListener() {
    RegisterPostLoadFunction(this);
    
}
void LocationDiscoveryCondition::SetConditionParameters(std::string locationName_a, std::string worldspaceID_a, std::string formID_a) {
    this->locationName = locationName_a;
    this->worldspaceID = worldspaceID_a;
    this->formID = formID_a;
}
bool LocationDiscoveryCondition::CheckCondition() {
	return false;
}

void LocationDiscoveryCondition::OnLocationDiscoveryEvent(const RE::LocationDiscovery::Event* event) {
	if (!event || !event->mapMarkerData) return;
	
	bool matched = false;
	if (this->formID != "") {
		RE::TESObjectREFR* refr = FindMapMarkerByData(event->mapMarkerData);
		if (this->cachedRef && refr && refr->GetFormID() == this->cachedRef->GetFormID()) {
			matched = true;
		}
	} else {
		std::string name = event->mapMarkerData->locationName.GetFullName();
		if (name == this->locationName) {
			matched = true;
		}
	}
	
	if (matched) {
		logger::info("Player met condition found {} in {}.", this->formID != "" ? this->formID : this->locationName, this->worldspaceID);
		this->UnlockNotify();
	}
}


LocationDiscoveryConditionFactory::LocationDiscoveryConditionFactory() : ConditionFactory() {};
Condition* LocationDiscoveryConditionFactory::createCondition() {
	return new LocationDiscoveryCondition();
};