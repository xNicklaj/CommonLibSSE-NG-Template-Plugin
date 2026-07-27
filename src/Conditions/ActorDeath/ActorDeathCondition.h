#include "../Condition.h"

class ActorDeathCondition : public Condition {
public:
	ActorDeathCondition();
	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(std::string) override;
	bool CheckCondition() override;

	std::string formID;
	RE::TESForm* cachedForm = nullptr;
private:
	
};

class ActorDeathConditionFactory : public ConditionFactory {
public:
	ActorDeathConditionFactory();
	Condition* createCondition() override;
};