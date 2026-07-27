#include "../Condition.h"

class BaseActorDeathCondition : public Condition {
public:
	BaseActorDeathCondition();
	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(std::string, int) override;
    void SetEditorID(bool);
	bool CheckCondition() override;
	void OnDeathEvent(const RE::TESDeathEvent* event) override;

	int Serialize() override;
	bool Deserialize(int value) override;

	void Localize(std::string path) override;

	std::string identifier = "";
	std::string name = "";
	int quantity = 0;
	RE::TESNPC* cachedNPC = nullptr;
private:
	
	int currQuantity = 0;
	bool isFormID = false;
    bool isEditorID = false;
};

class BaseActorDeathConditionFactory : public ConditionFactory {
public:
	BaseActorDeathConditionFactory();
	Condition* createCondition() override;
};