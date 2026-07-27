#include "../Condition.h"

class QuestStageDoneCondition : public Condition {
public:
	QuestStageDoneCondition();

	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(std::string formID, std::string, int stage) override;
	bool CheckCondition() override;

	std::string formID;
	int stage = 200;
	std::string OP;
	RE::TESQuest* cachedQuest = nullptr;
private:
	
};

class QuestStageDoneConditionFactory : public ConditionFactory {
public:
	QuestStageDoneConditionFactory();
	Condition* createCondition() override;
};