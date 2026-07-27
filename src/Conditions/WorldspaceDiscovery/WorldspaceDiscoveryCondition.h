#include "../Condition.h"
#include "../LocationDiscovery/LocationDiscoveryCondition.h"

#ifndef WORLDSPACEDISCOVERYCONDITION_H
#define WORLDSPACEDISCOVERYCONDITION_H

class WorldspaceDiscoveryCondition : public Condition {
public:
	WorldspaceDiscoveryCondition();
	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(std::string worldspaceID_a, int requiredCount_a);
	bool CheckCondition(void) override;

	

	std::string worldspaceID;
	int requiredCount;
};

class WorldspaceDiscoveryConditionFactory : public ConditionFactory {
public:
	WorldspaceDiscoveryConditionFactory();
	Condition* createCondition() override;
};

#endif
