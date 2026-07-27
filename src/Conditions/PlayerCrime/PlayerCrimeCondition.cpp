#include "PlayerCrimeCondition.h"

extern void RegisterPostLoadFunction(Condition* condition);

PlayerCrimeCondition::PlayerCrimeCondition() : Condition(ConditionType::NotSet) {}

void PlayerCrimeCondition::OnDataLoaded(void) {
	CheckCondition();
}

void PlayerCrimeCondition::EnableListener() {
	RegisterPostLoadFunction(this);
	RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(this);
}

void PlayerCrimeCondition::SetConditionParameters(std::vector<std::string> factionIDs_a, int bountyThreshold_a) {
	this->factionIDs = factionIDs_a;
	this->bountyThreshold = bountyThreshold_a;
}

bool PlayerCrimeCondition::CheckCondition() {
	RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
	if (!player) return false;

	bool allMet = true;
	for (const std::string& factionIDStr : this->factionIDs) {
		RE::TESFaction* faction = static_cast<RE::TESFaction*>(GetForm(factionIDStr, this->plugin));
		if (faction) {
            uint32_t currentBounty = player->GetCrimeGoldValue(faction);
            logger::debug("Faction {} has bounty {}", factionIDStr, currentBounty);
			if (currentBounty < this->bountyThreshold) {
				allMet = false;
				break;
			}
		} else {
            logger::error("Failed to find faction {}", factionIDStr);
			// If a faction isn't found, it can't have a bounty, so we haven't met the condition.
			allMet = false;
			break;
		}
	}

	if (allMet && this->factionIDs.size() > 0) {
		logger::info("Player met condition: Faction bounties reached threshold.");
		this->UnlockNotify();
		RE::ScriptEventSourceHolder::GetSingleton()->RemoveEventSink(this);
		return true;
	}

	return false;
}

RE::BSEventNotifyControl PlayerCrimeCondition::ProcessEvent(const RE::TESTrackedStatsEvent* a_event, RE::BSTEventSource<RE::TESTrackedStatsEvent>*) {
	std::string statName = a_event->stat.c_str();
	logger::debug("Tracked stat event: {}", statName);
	if (statName.find("Bounty") != std::string::npos) {
		CheckCondition();
	}
	return RE::BSEventNotifyControl::kContinue;
}

PlayerCrimeConditionFactory::PlayerCrimeConditionFactory() : ConditionFactory() {}

Condition* PlayerCrimeConditionFactory::createCondition() {
	return new PlayerCrimeCondition();
}
