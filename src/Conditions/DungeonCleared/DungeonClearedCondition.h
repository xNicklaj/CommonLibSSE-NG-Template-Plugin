#include "../Condition.h"

class DungeonClearedCondition : public Condition {
public:
	DungeonClearedCondition();

	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(std::string formid) override;
	bool CheckCondition();

	std::string formid;
	RE::BGSLocation* cachedLoc = nullptr;
private:
	
};

class DungeonClearedConditionFactory : public ConditionFactory {
public:
	DungeonClearedConditionFactory();
	Condition* createCondition() override;
};