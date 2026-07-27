#include "ItemCraftedCondition.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);

ItemCraftedCondition::ItemCraftedCondition() : Condition(ConditionType::ItemCrafted) {}
void ItemCraftedCondition::OnDataLoaded(void) {
    this->cachedForm = GetForm(this->itemID, this->plugin);
	if (this->cachedForm) ConditionManager::GetSingleton()->RegisterItemCraftedListener(this->cachedForm->formID, this);
}
void ItemCraftedCondition::EnableListener() {
	RegisterPostLoadFunction(this);
	
}
void ItemCraftedCondition::SetConditionParameters(std::string itemID__a) {
	this->itemID = itemID__a;
}
bool ItemCraftedCondition::CheckCondition() {
	logger::debug("Player met condition crafted item {}", this->itemID);
	this->UnlockNotify();
	return true;
}


ItemCraftedConditionFactory::ItemCraftedConditionFactory() : ConditionFactory() {};
Condition* ItemCraftedConditionFactory::createCondition() {
	return new ItemCraftedCondition();
};