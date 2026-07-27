#include "GlobalVariableStateCondition.h"
#include "../../CommonFunctions.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);

GlobalVariableStateCondition::GlobalVariableStateCondition(void) : Condition(ConditionType::GlobalVariableState) {};
void GlobalVariableStateCondition::OnDataLoaded(void) {
	this->cachedGlobal = static_cast<RE::TESGlobal*>(GetForm(this->formID, this->plugin));
	ConditionManager::GetSingleton()->RegisterPositionPlayerListener(this);
	CheckCondition();
};
void GlobalVariableStateCondition::EnableListener(void) {
	RegisterPostLoadFunction(this);
};
void GlobalVariableStateCondition::SetConditionParameters(std::string formID_a, float value_a) {
	this->formID = formID_a;
	this->value = value_a;
};
bool GlobalVariableStateCondition::CheckCondition() {
	if(this->isMet) return true;
	if (!this->cachedGlobal) return false;
	float globValue = GetGlobalVariableValue(this->cachedGlobal->formID);
	if(globValue >= this->value) {
		this->UnlockNotify();
		
		return true;
	}
	return false;
};



// GlobalVariableStateConditionFactory
GlobalVariableStateConditionFactory::GlobalVariableStateConditionFactory() : ConditionFactory() {};
Condition* GlobalVariableStateConditionFactory::createCondition() {
	return new GlobalVariableStateCondition();
};