#include "../Condition.h"

class PlayerFirstEnterCellCondition : public Condition {
public:
	PlayerFirstEnterCellCondition();
	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(std::string cellID) override;
	bool CheckCondition();
	std::string cellID;
	RE::TESForm* cachedCell = nullptr;
private:
	
};

class PlayerFirstEnterCellConditionFactory : public ConditionFactory {
public:
	PlayerFirstEnterCellConditionFactory();
	Condition* createCondition() override;
};