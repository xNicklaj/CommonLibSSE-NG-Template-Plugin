#ifndef LOCATIONDISCOVERYCONDITION_H
#define LOCATIONDISCOVERYCONDITION_H

#include "../Condition.h"
// CANNOT USE DUE TO IT NOT BEING MAPPED INSIDE RE

class LocationDiscoveryCondition : public Condition {
public:
	LocationDiscoveryCondition();
	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(std::string locationName_a, std::string worldspaceID_a, std::string formID_a);
	bool CheckCondition() override;
	void OnLocationDiscoveryEvent(const RE::LocationDiscovery::Event* event) override;

	void Localize(std::string path) override;

	std::string locationName;
	std::string worldspaceID;
	std::string formID;
	RE::TESObjectREFR* cachedRef = nullptr;
private:
	
};

class LocationDiscoveryConditionFactory : public ConditionFactory {
public:
	LocationDiscoveryConditionFactory();
	Condition* createCondition() override;
};

#endif