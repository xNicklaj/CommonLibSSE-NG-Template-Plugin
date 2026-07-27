#include "PlayerLevelCondition.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);

PlayerLevelCondition::PlayerLevelCondition() : Condition(ConditionType::PlayerLevel) {}
void PlayerLevelCondition::OnDataLoaded(void) {
	ConditionManager::GetSingleton()->RegisterLevelListener(this);
    CheckCondition();
}
void PlayerLevelCondition::EnableListener(void)
{
    RegisterPostLoadFunction(this);
    
}
void PlayerLevelCondition::SetConditionParameters(int level_a) {
    this->level = level_a;
}

bool PlayerLevelCondition::CheckCondition() {
    if (!this->isMet && RE::PlayerCharacter::GetSingleton()->GetLevel() >= level) {
        this->UnlockNotify();
        
        return true;
    }
    return false;
}

PlayerLevelConditionFactory::PlayerLevelConditionFactory() : ConditionFactory() {};
Condition* PlayerLevelConditionFactory::createCondition() {
    return new PlayerLevelCondition();
};