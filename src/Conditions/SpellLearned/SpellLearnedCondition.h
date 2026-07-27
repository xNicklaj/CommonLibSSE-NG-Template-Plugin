#include "../Condition.h"

class SpellLearnedCondition : public Condition
{
public:
	SpellLearnedCondition();
	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(std::string skill) override;
	bool CheckCondition() override;

	std::string FormID;
	RE::SpellItem* cachedSpell = nullptr;
private:
	
};

class SpellLearnedConditionFactory : public ConditionFactory
{
public:
	SpellLearnedConditionFactory();
	Condition* createCondition() override;
};