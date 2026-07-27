#include "../Condition.h"
#include <vector>

#ifndef PLAYERCRIMECONDITION_H
#define PLAYERCRIMECONDITION_H

class PlayerCrimeCondition : public Condition {
public:
	PlayerCrimeCondition(void);
	void EnableListener(void) override;
	void OnDataLoaded(void) override;
	void SetConditionParameters(std::vector<std::string> factionIDs_a, int bountyThreshold_a) override;
	bool CheckCondition(void) override;

	

	std::vector<std::string> factionIDs;
	int bountyThreshold;
	std::vector<RE::TESFaction*> cachedFactions;
};

class PlayerCrimeConditionFactory : public ConditionFactory {
public:
	PlayerCrimeConditionFactory();
	Condition* createCondition() override;
};

#endif