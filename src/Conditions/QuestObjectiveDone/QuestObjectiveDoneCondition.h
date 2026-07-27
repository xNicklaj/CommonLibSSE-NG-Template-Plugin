#include "../Condition.h"

class QuestObjectiveDoneCondition : public Condition {
public:
	QuestObjectiveDoneCondition();

	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(std::string formID, int objective) override;
	bool CheckCondition() override;

	std::string formID;
	int objective = 200;
private:
	
	
	RE::TESQuest* quest = nullptr;
};

class QuestObjectiveDoneConditionFactory : public ConditionFactory {
public:
	QuestObjectiveDoneConditionFactory();
	Condition* createCondition() override;
};