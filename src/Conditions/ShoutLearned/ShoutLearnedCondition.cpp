#include "ShoutLearnedCondition.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);

ShoutLearnedCondition::ShoutLearnedCondition() : Condition(ConditionType::SpellLearned) {}
void ShoutLearnedCondition::OnDataLoaded(void) {
	this->cachedShout = static_cast<RE::TESShout*>(GetForm(this->FormID, this->plugin));
	ConditionManager::GetSingleton()->RegisterTrackedStatListener("Words Of Power Learned", this);
    CheckCondition();
}
void ShoutLearnedCondition::EnableListener(void)
{
    RegisterPostLoadFunction(this);
    
}
void ShoutLearnedCondition::SetConditionParameters(std::string formID_a, int wordNumber_a) {
    this->FormID = formID_a;
    this->wordNumber = wordNumber_a;
}


bool ShoutLearnedCondition::CheckCondition() {
    if (this->isMet) return true;
    if (!this->cachedShout) return false;

    bool found = false;
    RE::TESShout* target = this->cachedShout;
    RE::TESNPC* player = RE::PlayerCharacter::GetSingleton()->GetActorBase();
    if(!target) return false;

    int wordCount = 0;
    if (!found) {
        for (unsigned i = 0; i < player->GetSpellList()->numShouts; i++) {
            RE::TESShout* shout = player->GetSpellList()->shouts[i];
            if (shout && shout->formID == target->formID) {
                found = true;
                for(const auto& variation : shout->variations){
                    if(variation.word->GetKnown()) 
                        wordCount++;
				}
                logger::debug("Shouts {} has learned {}/3 words. ({} required)", shout->fullName.c_str(), wordCount, this->wordNumber);
                break;
            }
        }
    }
    
    if (found && wordCount >= this->wordNumber) {
        logger::debug("Player met condition learned shout {}:{}", target->fullName.c_str(), FormIDToString(target->formID));
        this->UnlockNotify();
        
    }
    return false;
}

ShoutLearnedConditionFactory::ShoutLearnedConditionFactory() : ConditionFactory() {};
Condition* ShoutLearnedConditionFactory::createCondition() {
    return new ShoutLearnedCondition();
}