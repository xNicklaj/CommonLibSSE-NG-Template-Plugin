#include "QuestStageDoneCondition.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);

QuestStageDoneCondition::QuestStageDoneCondition() : Condition(ConditionType::QuestObjectiveDone) {}
void QuestStageDoneCondition::SetConditionParameters(std::string formID_a, std::string OP_a, int stage_a) {
    this->formID = formID_a;
    this->OP = OP_a;
    this->stage = stage_a;
}
void QuestStageDoneCondition::OnDataLoaded(void) {
    this->cachedQuest = static_cast<RE::TESQuest*>(GetForm(this->formID, this->plugin));
	if (this->cachedQuest) ConditionManager::GetSingleton()->RegisterQuestListener(this->cachedQuest->formID, this);
    CheckCondition();
}
void QuestStageDoneCondition::EnableListener(void)
{
    RegisterPostLoadFunction(this);

    // Bind sink for quest stage change event
    
}

bool QuestStageDoneCondition::CheckCondition() {
    if (this->isMet) return true;
    if ((this->OP == ">=" || this->OP == "GT") && (this->cachedQuest ? this->cachedQuest->GetCurrentStageID() : 0) >= stage) {
        logger::debug("Quest {} met condition stage {}", this->formID, this->stage);
        this->UnlockNotify();
        
        return true;
    }
    else if ((this->OP == "==" || this->OP == "EQ") && (this->cachedQuest ? this->cachedQuest->GetCurrentStageID() : 0) == stage) {
        logger::debug("Quest {} met condition stage {}", this->formID, this->stage);
        this->UnlockNotify();
        
        return true;
    }
    return false;
}

QuestStageDoneConditionFactory::QuestStageDoneConditionFactory() : ConditionFactory() {};
Condition* QuestStageDoneConditionFactory::createCondition() {
    return new QuestStageDoneCondition();
};