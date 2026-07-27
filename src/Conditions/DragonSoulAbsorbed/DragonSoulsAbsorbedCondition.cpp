#include "DragonSoulsAbsorbedCondition.h"
#include "../../ConditionManager.h"

extern void RegisterPostLoadFunction(Condition* condition);

DragonSoulAbsorbedCondition::DragonSoulAbsorbedCondition() : Condition(ConditionType::DragonSoulAbsorbed) {}
void DragonSoulAbsorbedCondition::OnDataLoaded(void) {
	ConditionManager::GetSingleton()->RegisterDragonSoulListener(this);
    CheckCondition();
}
void DragonSoulAbsorbedCondition::EnableListener() {
    RegisterPostLoadFunction(this);
    
}
void DragonSoulAbsorbedCondition::SetConditionParameters(int quantity_a) {
    this->quantity = quantity_a;
}
bool DragonSoulAbsorbedCondition::CheckCondition() {
    RE::TESGlobal* absorbed = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESGlobal>(0x1C0F2, "Skyrim.esm");
    float totalAbsorbed = absorbed->value;
    if (totalAbsorbed >= quantity) {
        logger::info("Player met condition absorbed souls {}", this->quantity);
        this->UnlockNotify();
        
        return true;
    }
    return false;
}


DragonSoulAbsorbedConditionFactory::DragonSoulAbsorbedConditionFactory() : ConditionFactory() {};
Condition* DragonSoulAbsorbedConditionFactory::createCondition() {
    return new DragonSoulAbsorbedCondition();
};
