#include "../Condition.h"

class DragonSoulAbsorbedCondition : public Condition {
public:
	DragonSoulAbsorbedCondition();
	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(int quantity) override;
	bool CheckCondition() override;

	int quantity = 0;
private:
	
};

class DragonSoulAbsorbedConditionFactory : public ConditionFactory {
public:
	DragonSoulAbsorbedConditionFactory();
	Condition* createCondition() override;
};