#include "ActorDeathCondition.h"
#include "../../ConditionManager.h"


extern void RegisterPostLoadFunction(Condition* condition);

ActorDeathCondition::ActorDeathCondition() : Condition(ConditionType::ActorDeath){};
void ActorDeathCondition::OnDataLoaded(void) {
	// TODO check if actor is already dead
	this->cachedForm = GetForm(this->formID, this->plugin);
	auto* target = this->cachedForm;
	if (target == NULL) return;
	RE::TESObjectREFR* targetREFR = RE::TESObjectREFR::LookupByID<RE::TESObjectREFR>(target->formID);
	if(targetREFR && targetREFR->IsDead()) {
		this->UnlockNotify();
		
	}
	ConditionManager::GetSingleton()->RegisterDeathListener(this->cachedForm->formID, this);
}
void ActorDeathCondition::EnableListener(void) {
	RegisterPostLoadFunction(this);
	
};
void ActorDeathCondition::SetConditionParameters(std::string formID_a) {
	this->formID = formID_a;
};
bool ActorDeathCondition::CheckCondition() {
    if (this->isMet) return true;
    RE::Actor* actor = this->cachedForm ? this->cachedForm->As<RE::Actor>() : nullptr;
    if (!actor) return false;
    if (actor->IsDead()) {
        logger::debug("Player met condition dead form {}", this->formID);
        this->UnlockNotify();
        return true;
    }
    return false;
};
;

ActorDeathConditionFactory::ActorDeathConditionFactory() : ConditionFactory() {};
Condition* ActorDeathConditionFactory::createCondition() {
	return new ActorDeathCondition();
};