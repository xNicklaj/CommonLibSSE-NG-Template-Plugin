#include "PlayerCrimeCondition.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);

PlayerCrimeCondition::PlayerCrimeCondition() : Condition(ConditionType::NotSet) {}

void PlayerCrimeCondition::OnDataLoaded(void) {
	for (const auto& id : this->factionIDs) {
		auto faction = static_cast<RE::TESFaction*>(GetForm(id, this->plugin));
		if(faction) this->cachedFactions.push_back(faction);
	}
	ConditionManager::GetSingleton()->RegisterTrackedStatListener("Bounty", this); ConditionManager::GetSingleton()->RegisterTrackedStatListener("Total Fines", this);
	CheckCondition();
}

void PlayerCrimeCondition::EnableListener() {
	RegisterPostLoadFunction(this);
	
}

void PlayerCrimeCondition::SetConditionParameters(std::vector<std::string> factionIDs_a, int bountyThreshold_a) {
	this->factionIDs = factionIDs_a;
	this->bountyThreshold = bountyThreshold_a;
}

bool PlayerCrimeCondition::CheckCondition() {
	RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
	if (!player) return false;

	bool allMet = true;
	for (auto faction : this->cachedFactions) {
		uint32_t currentBounty = player->GetCrimeGoldValue(faction);
		if (currentBounty < this->bountyThreshold) {
			allMet = false;
			break;
		}
	}
	if (this->cachedFactions.size() != this->factionIDs.size()) allMet = false;

	if (allMet && this->cachedFactions.size() > 0) {
		logger::debug("Player met condition: Faction bounties reached threshold.");
		this->UnlockNotify();
		
		return true;
	}

	return false;
}



PlayerCrimeConditionFactory::PlayerCrimeConditionFactory() : ConditionFactory() {}

Condition* PlayerCrimeConditionFactory::createCondition() {
	return new PlayerCrimeCondition();
}
