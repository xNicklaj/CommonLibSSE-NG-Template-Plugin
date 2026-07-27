#include "BookReadCondition.h"
#include "../../CommonFunctions.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);

BookReadCondition::BookReadCondition() : Condition(ConditionType::BookRead) {}

void BookReadCondition::OnDataLoaded(void) {
    if (isHex(this->identifier)) this->cachedBook = GetForm(this->identifier, this->plugin);
	if(isHex(this->identifier) && this->cachedBook) ConditionManager::GetSingleton()->RegisterBookReadListener(this->cachedBook->formID, this); 
	else if (!isHex(this->identifier)) ConditionManager::GetSingleton()->RegisterBookReadListener(0, this);
    CheckCondition();
}
void BookReadCondition::EnableListener() {
    RegisterPostLoadFunction(this);
    
}
void BookReadCondition::SetConditionParameters(std::string identifier_a) {
    this->identifier = identifier_a;
}
void BookReadCondition::Localize(std::string path) {
    if(this->identifier[0] == '$')
		this->identifier = LocalizationManager::GetSingleton()->GetLocalizedText(path, LocalizationManager::GetSingleton()->CurrentLocale(), this->identifier);
}
bool BookReadCondition::CheckCondition() {
	logger::debug("Player met condition read book {}.", this->identifier);
	this->UnlockNotify();
	return true;
}

void BookReadCondition::OnBookReadEvent(const RE::BooksRead::Event* event) {
	if (!event || !event->book) return;
	
	if (!isHex(this->identifier)) {
		std::string title = event->book->GetFullName();
		if (title == this->identifier) {
			this->CheckCondition();
		}
	}
}


BookReadConditionFactory::BookReadConditionFactory() : ConditionFactory() {};
Condition* BookReadConditionFactory::createCondition() {
	return new BookReadCondition();
};