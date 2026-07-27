#include "SpellLearnedCondition.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);

SpellLearnedCondition::SpellLearnedCondition() : Condition(ConditionType::SpellLearned) {}
void SpellLearnedCondition::OnDataLoaded(void) {
    this->cachedSpell = static_cast<RE::SpellItem*>(GetForm(this->FormID, this->plugin));
	if (this->cachedSpell) ConditionManager::GetSingleton()->RegisterSpellLearnedListener(this->cachedSpell->formID, this);
    CheckCondition();
}
void SpellLearnedCondition::EnableListener(void)
{
    RegisterPostLoadFunction(this);
    
}
void SpellLearnedCondition::SetConditionParameters(std::string formID) {
    this->FormID = formID;
}


bool SpellLearnedCondition::CheckCondition() {
    if (this->isMet) return true;
    if (!this->cachedSpell) return false;

    bool found = false;
    RE::SpellItem* target = this->cachedSpell;
    RE::TESNPC* player = RE::PlayerCharacter::GetSingleton()->GetActorBase();
    
    if (target && RE::PlayerCharacter::GetSingleton()->HasSpell(target))
        found = true;

    if (!found) {
        for (unsigned i = 0; i < player->GetSpellList()->numSpells; i++) {
            RE::SpellItem* spell = player->GetSpellList()->spells[i];
            if (spell && spell->formID == target->formID) {
                found = true;
                break;
            }
        }
    }
    
    if (found) {
        logger::debug("Player met condition learned {}:{}", target->fullName.c_str(), FormIDToString(target->formID));
        this->UnlockNotify();
        
    }
    return false;
}

SpellLearnedConditionFactory::SpellLearnedConditionFactory() : ConditionFactory() {};
Condition* SpellLearnedConditionFactory::createCondition() {
    return new SpellLearnedCondition();
}