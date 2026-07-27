#include "../Condition.h"

class ItemCraftedCondition : public Condition {
public:
	ItemCraftedCondition();
	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(std::string itemID) override;
	bool CheckCondition() override;
	
	std::string itemID;
	RE::TESForm* cachedForm = nullptr;
private:
	
};

class ItemCraftedConditionFactory : public ConditionFactory {
	public:
	ItemCraftedConditionFactory();
	Condition* createCondition() override;
};
