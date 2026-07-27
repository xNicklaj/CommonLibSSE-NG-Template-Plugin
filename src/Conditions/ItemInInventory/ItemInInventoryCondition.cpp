#include "ItemInInventoryCondition.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);

ItemInInventoryCondition::ItemInInventoryCondition() : Condition(ConditionType::ItemInInventory) {}
void ItemInInventoryCondition::OnDataLoaded(void) {
	this->cachedForm = GetForm(this->formid, this->plugin);
	if (this->cachedForm) ConditionManager::GetSingleton()->RegisterItemInInventoryListener(this->cachedForm->formID, this);
    CheckCondition();
}
void ItemInInventoryCondition::EnableListener() {
    RegisterPostLoadFunction(this);
}
void ItemInInventoryCondition::SetConditionParameters(std::string formid_a, int quantity_a) {
    this->formid = formid_a;
    this->quantity = quantity_a;
}

bool ItemInInventoryCondition::CheckCondition() {
    //RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
    int quantity_l = 0;

    const auto inventory = RE::PlayerCharacter::GetSingleton()->GetInventory();
    RE::TESForm* target;
    for (auto& [item, data] : inventory) {
        // For fucking reasons data.first is the amount. Ok
        target = this->cachedForm;
        if (item != NULL && target != NULL && item->GetFormID() == target->formID) {
            quantity_l += data.first;
        }
    }
    if (quantity_l >= this->quantity) {
        logger::info("Player met condition item {} quantity {}", this->formid, this->quantity);
        this->UnlockNotify();
        
        return true;
    }
    return false;
}

ItemInInventoryConditionFactory::ItemInInventoryConditionFactory() : ConditionFactory() {};
Condition* ItemInInventoryConditionFactory::createCondition() {
    return new ItemInInventoryCondition();
};