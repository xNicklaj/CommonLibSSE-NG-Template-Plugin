#include "QueryStatValueCondition.h"
#include "../../ConditionManager.h"
#include "../../CustomRE.h"

extern void RegisterPostLoadFunction(Condition* condition);

QueryStatValueCondition::QueryStatValueCondition(void) : Condition(ConditionType::QueryStatValue) {};
void QueryStatValueCondition::OnDataLoaded(void) {
	ConditionManager::GetSingleton()->RegisterTrackedStatListener(this->stat, this);
	CheckCondition();
};
void QueryStatValueCondition::EnableListener(void) {
	RegisterPostLoadFunction(this);
	
};
void QueryStatValueCondition::SetConditionParameters(std::string stat_a, float value_a) {
	this->stat = stat_a;
	this->value = value_a;
};
bool QueryStatValueCondition::CheckCondition() {
	int32_t globValue = RE::QueryStat(this->stat.c_str());
	if (globValue >= this->value) {
		this->UnlockNotify();
		return true;
	}
	return false;
};
;

// GlobalVariableStateConditionFactory
QueryStatValueConditionFactory::QueryStatValueConditionFactory() : ConditionFactory() {};
Condition* QueryStatValueConditionFactory::createCondition() {
	return new QueryStatValueCondition();
};