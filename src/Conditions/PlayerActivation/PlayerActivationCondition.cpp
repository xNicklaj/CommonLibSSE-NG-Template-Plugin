#include "PlayerActivationCondition.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);

PlayerActivationCondition::PlayerActivationCondition() : Condition(PlayerActivation) {}
void PlayerActivationCondition::OnDataLoaded(void) {
	this->cachedForm = GetForm(this->formid, this->plugin);
	if (this->cachedForm) {
		ConditionManager::GetSingleton()->RegisterActivateListener(this->cachedForm->formID, this);
	}
};
void PlayerActivationCondition::EnableListener(void) {
	RegisterPostLoadFunction(this);
	
};
void PlayerActivationCondition::SetConditionParameters(std::string formid_a) {
	this->formid = formid_a;
};
bool PlayerActivationCondition::CheckCondition() {
	logger::debug("Player met condition activation {}", this->formid);
	this->UnlockNotify();
	return true;
};
;

PlayerActivationConditionFactory::PlayerActivationConditionFactory() : ConditionFactory() {};
Condition* PlayerActivationConditionFactory::createCondition() {
	return new PlayerActivationCondition();
};