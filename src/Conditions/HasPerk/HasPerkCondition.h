#ifndef HASPERK_CONDITION_H
#define HASPERK_CONDITION_H

#include "../Condition.h"
#include "../../CommonFunctions.h"

class HasPerkCondition : public Condition {
public:
	HasPerkCondition();

	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(std::string formID) override;
	bool CheckCondition() override;

	std::string formIDStr;
	RE::BGSPerk* perk = nullptr;
private:
	
};

class HasPerkConditionFactory : public ConditionFactory {
public:
	HasPerkConditionFactory();
	Condition* createCondition() override;
};

#endif // HASPERK_CONDITION_H
