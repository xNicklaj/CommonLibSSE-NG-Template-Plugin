#include "WorldspaceDiscoveryCondition.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);
extern RE::BSTArray<RE::ObjectRefHandle>* GetPlayerMapMarkers();

WorldspaceDiscoveryCondition::WorldspaceDiscoveryCondition() : Condition(ConditionType::NotSet) {}

void WorldspaceDiscoveryCondition::OnDataLoaded(void) {
	ConditionManager::GetSingleton()->RegisterLocationDiscoveryListener(this);
	CheckCondition();
}

void WorldspaceDiscoveryCondition::EnableListener() {
	RegisterPostLoadFunction(this);
	
}

void WorldspaceDiscoveryCondition::SetConditionParameters(std::string worldspaceID_a, int requiredCount_a) {
	this->worldspaceID = worldspaceID_a;
	this->requiredCount = requiredCount_a;
}

bool WorldspaceDiscoveryCondition::CheckCondition() {
	auto* playerMapMarkers = GetPlayerMapMarkers();
	int count = 0;
	
	for (auto playerMapMarker : *playerMapMarkers) {
		const auto refr = playerMapMarker.get().get();
		if (refr && !refr->IsDisabled()) {
			const auto marker = refr->extraList.GetByType<RE::ExtraMapMarker>();
			if (marker && marker->mapData) {
				if (marker->mapData->flags.any(RE::MapMarkerData::Flag::kCanTravelTo)) {
					auto worldspace = refr->GetWorldspace();
					if (worldspace) {
						std::string wsEditorID = clib_util::editorID::get_editorID(worldspace);
						if (wsEditorID == this->worldspaceID) {
							count++;
						}
					}
				}
			}
		}
	}
	
	if (count >= this->requiredCount && this->requiredCount > 0) {
		logger::debug("Player met condition: Discovered {} locations in {}.", this->requiredCount, this->worldspaceID);
		this->UnlockNotify();
		
		return true;
	}
	
	return false;
}



WorldspaceDiscoveryConditionFactory::WorldspaceDiscoveryConditionFactory() : ConditionFactory() {}

Condition* WorldspaceDiscoveryConditionFactory::createCondition() {
	return new WorldspaceDiscoveryCondition();
}
