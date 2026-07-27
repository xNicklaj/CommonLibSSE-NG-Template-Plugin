#include "../Condition.h"

class PlayerLevelCondition : public Condition {
public:
	PlayerLevelCondition();

	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(int level) override;
	bool CheckCondition() override;

	int level = 0;
private:
	
};

class PlayerLevelConditionFactory : public ConditionFactory {
public:
	PlayerLevelConditionFactory();
	Condition* createCondition() override;
};