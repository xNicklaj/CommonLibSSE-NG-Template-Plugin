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
			if (player->GetCrimeGoldValue(faction) < this->bountyThreshold) {
				allMet = false;
				break;
			}
		} else {
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
	if (a_event->stat == "Total Bounty") {
		CheckCondition();
	}
	return RE::BSEventNotifyControl::kContinue;
}

PlayerCrimeConditionFactory::PlayerCrimeConditionFactory() : ConditionFactory() {}

Condition* PlayerCrimeConditionFactory::createCondition() {
	return new PlayerCrimeCondition();
}
