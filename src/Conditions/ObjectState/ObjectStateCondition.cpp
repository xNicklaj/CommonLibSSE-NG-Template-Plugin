#include "ObjectStateCondition.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);

ObjectStateCondition::ObjectStateCondition(void) : Condition(ConditionType::ObjectState){};
void ObjectStateCondition::OnDataLoaded(void) {
	ConditionManager::GetSingleton()->RegisterPositionPlayerListener(this);
    this->CheckCondition();
};
void ObjectStateCondition::EnableListener(void) {
    RegisterPostLoadFunction(this);
};
void ObjectStateCondition::SetConditionParameters(std::string formID_a, bool state_a) {
    this->formID = formID_a;
    this->state = state_a;
};
bool ObjectStateCondition::CheckCondition() {
    if (this->isMet)
        return true;
    RE::TESObjectREFR* target = static_cast<RE::TESObjectREFR*>(GetForm(this->formID, this->plugin));
    if (!target)
        return false;
    logger::debug("Object {} is {}.", this->formID, !target->IsDisabled() ? "ENABLED" : "DISABLED");
    if (target->IsDisabled() == !this->state) {
        this->UnlockNotify();
        
        return true;
    }
    return false;
};



ObjectStateConditionFactory::ObjectStateConditionFactory()
    : ConditionFactory(){};
Condition* ObjectStateConditionFactory::createCondition() {
    return new ObjectStateCondition();
};