#include "DungeonClearedCondition.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);

extern void RegisterPostLoadFunction(Condition* condition);

DungeonClearedCondition::DungeonClearedCondition() : Condition(ConditionType::LocationCleared) {}
void DungeonClearedCondition::OnDataLoaded(void) {
    this->cachedLoc = static_cast<RE::BGSLocation*>(GetForm(this->formid, this->plugin));
	ConditionManager::GetSingleton()->RegisterTrackedStatListener("Dungeons Cleared", this);
    CheckCondition();
}
void DungeonClearedCondition::EnableListener() {
    RegisterPostLoadFunction(this);
    
}
void DungeonClearedCondition::SetConditionParameters(std::string formid_a) {
    this->formid = formid_a;
}
bool DungeonClearedCondition::CheckCondition() {
    RE::BGSLocation* target = this->cachedLoc;
    if (!target) return false;
    if (target->IsCleared()) {
        logger::debug("Player met condition location {} cleared.", this->formid);
        UnlockNotify();
        
        return true;
    }
    return false;
}


DungeonClearedConditionFactory::DungeonClearedConditionFactory() : ConditionFactory() {};
Condition* DungeonClearedConditionFactory::createCondition() {
    return new DungeonClearedCondition();
};