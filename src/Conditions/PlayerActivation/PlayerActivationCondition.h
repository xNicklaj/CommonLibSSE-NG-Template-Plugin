#include "../Condition.h"

class PlayerActivationCondition : public Condition {
public:
	PlayerActivationCondition();
	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(std::string formid) override;
	bool CheckCondition() override;

	std::string formid;
	RE::TESForm* cachedForm = nullptr;
private:
	
};

class PlayerActivationConditionFactory : public ConditionFactory {
public:
	PlayerActivationConditionFactory();
	Condition* createCondition() override;
};