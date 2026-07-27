#include "HasPerkCondition.h"
#include "../../ConditionManager.h"

HasPerkCondition::HasPerkCondition(void) : Condition(ConditionType::HasPerk) {}

void HasPerkCondition::SetConditionParameters(std::string formID) {
	this->formIDStr = formID;
}

void HasPerkCondition::EnableListener(void) {
	
}

void HasPerkCondition::OnDataLoaded(void) {
	this->perk = static_cast<RE::BGSPerk*>(GetForm(this->formIDStr, this->plugin));
	if (!this->perk) {
		logger::error("Failed to find perk {}", this->formIDStr);
	}
	ConditionManager::GetSingleton()->RegisterMenuListener(this);
	CheckCondition();
}

bool HasPerkCondition::CheckCondition() {
	if (this->isMet) return false;
	
	if (!this->perk) {
		this->perk = static_cast<RE::BGSPerk*>(GetForm(this->formIDStr, this->plugin));
	}
	
	if (!this->perk) return false;
	
	auto player = RE::PlayerCharacter::GetSingleton();
	if (player && player->HasPerk(this->perk)) {
		this->UnlockNotify();
		
		return true;
	}
	
	return false;
}



HasPerkConditionFactory::HasPerkConditionFactory() : ConditionFactory() {}

Condition* HasPerkConditionFactory::createCondition() {
	return new HasPerkCondition();
}
