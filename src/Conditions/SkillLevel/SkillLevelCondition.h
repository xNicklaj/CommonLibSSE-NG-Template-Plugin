#include "../Condition.h"

class PlayerSkillLevelCondition : public Condition {
public:
	PlayerSkillLevelCondition();

	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(std::string skill, int level) override;
	bool CheckCondition() override;

	std::string skill;
	int level = 0;

private:
	
};

class PlayerSkillLevelConditionFactory : public ConditionFactory {
public:
	PlayerSkillLevelConditionFactory();
	Condition* createCondition() override;
};