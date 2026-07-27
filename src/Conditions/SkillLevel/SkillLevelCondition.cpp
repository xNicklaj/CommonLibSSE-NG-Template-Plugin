#include "SkillLevelCondition.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);

PlayerSkillLevelCondition::PlayerSkillLevelCondition() : Condition(ConditionType::PlayerLevel) {}
void PlayerSkillLevelCondition::OnDataLoaded(void) {
	ConditionManager::GetSingleton()->RegisterSkillListener(this);
    CheckCondition();
}
void PlayerSkillLevelCondition::EnableListener(void)
{
    RegisterPostLoadFunction(this);
    
}
void PlayerSkillLevelCondition::SetConditionParameters(std::string skill_a, int level_a) {
    this->level = level_a;
    this->skill = skill_a;
}

bool PlayerSkillLevelCondition::CheckCondition() {
    RE::ActorValue skill_l = StringToActorValue(this->skill);
    float skillLevel = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner()->GetActorValue(skill_l);
    if (!this->isMet && skillLevel >= this->level) {
        logger::info("Skill {} met condition level {}", this->skill, skillLevel);
        this->UnlockNotify();
        
        return true;
    }
    return false;
}

PlayerSkillLevelConditionFactory::PlayerSkillLevelConditionFactory() : ConditionFactory() {};
Condition* PlayerSkillLevelConditionFactory::createCondition() {
    return new PlayerSkillLevelCondition();
}