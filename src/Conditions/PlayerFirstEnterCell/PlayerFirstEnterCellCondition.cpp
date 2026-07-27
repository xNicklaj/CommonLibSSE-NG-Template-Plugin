#include "PlayerFirstEnterCellCondition.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);

PlayerFirstEnterCellCondition::PlayerFirstEnterCellCondition() : Condition(ConditionType::FirstEnterCell) {}
void PlayerFirstEnterCellCondition::SetConditionParameters(std::string cellID_a) {
    this->cellID = cellID_a;
}
void PlayerFirstEnterCellCondition::OnDataLoaded(void) {
	RE::TESForm* cell = GetForm(this->cellID, this->plugin);
	if (cell) ConditionManager::GetSingleton()->RegisterActorCellListener(cell->formID, this);
}
void PlayerFirstEnterCellCondition::EnableListener(void)
{
    RegisterPostLoadFunction(this);

    // Bind sink for quest stage change event
}

bool PlayerFirstEnterCellCondition::CheckCondition() {
    logger::info("Player met condition entered cell {}.", this->cellID);
    this->UnlockNotify();
    return true;
}

PlayerFirstEnterCellConditionFactory::PlayerFirstEnterCellConditionFactory() : ConditionFactory() {};
Condition* PlayerFirstEnterCellConditionFactory::createCondition() {
    return new PlayerFirstEnterCellCondition();
};