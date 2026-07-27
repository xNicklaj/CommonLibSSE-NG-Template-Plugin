#include "../Condition.h"

class ItemInInventoryCondition : public Condition {
public:
	ItemInInventoryCondition();

	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(std::string formid, int quantity) override;
	bool CheckCondition() override;

	std::string formid;
	int quantity = 1;
	RE::TESForm* cachedForm = nullptr;
private:
	
};

class ItemInInventoryConditionFactory : public ConditionFactory {
public:
	ItemInInventoryConditionFactory();
	Condition* createCondition() override;
};