#include "../Condition.h"

class BookReadCondition : public Condition {
public:
	BookReadCondition();
	void OnDataLoaded(void) override;
	void EnableListener(void) override;
	void SetConditionParameters(std::string formid) override;
	bool CheckCondition() override;
	void OnBookReadEvent(const RE::BooksRead::Event* event) override;
	void Localize(std::string path) override;

	std::string identifier;
	RE::TESForm* cachedBook = nullptr;
private:
	
};
class BookReadConditionFactory : public ConditionFactory {
public:
	BookReadConditionFactory();
	Condition* createCondition() override;
};