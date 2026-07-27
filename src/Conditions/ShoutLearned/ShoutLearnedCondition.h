#include "../Condition.h"

class ShoutLearnedCondition : public Condition
{
public:
	ShoutLearnedCondition();
	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(std::string skill, int wordNumber) override;
	bool CheckCondition() override;

	std::string FormID;
	int wordNumber = 1;
	RE::TESShout* cachedShout = nullptr;
private:
	
};

class ShoutLearnedConditionFactory : public ConditionFactory
{
public:
	ShoutLearnedConditionFactory();
	Condition* createCondition() override;
};