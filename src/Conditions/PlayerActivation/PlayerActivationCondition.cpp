#include "PlayerActivationCondition.h"

extern void RegisterPostLoadFunction(Condition* condition);

PlayerActivationCondition::PlayerActivationCondition() : Condition(PlayerActivation) {}
void PlayerActivationCondition::OnDataLoaded(void) {
};
void PlayerActivationCondition::EnableListener(void) {
	RegisterPostLoadFunction(this);
	RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(this);
};
void PlayerActivationCondition::SetConditionParameters(std::string formid_a) {
	this->formid = formid_a;
};
bool PlayerActivationCondition::CheckCondition() {
	return false;
};
RE::BSEventNotifyControl PlayerActivationCondition::ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*) {
	if (this->cachedForm && a_event->objectActivated && a_event->objectActivated->GetFormID() == this->cachedForm->formID) {
		logger::info("Player met condition activation {}", this->formid);
		this->UnlockNotify();
	}
	return RE::BSEventNotifyControl::kContinue;
};

PlayerActivationConditionFactory::PlayerActivationConditionFactory() : ConditionFactory() {};
Condition* PlayerActivationConditionFactory::createCondition() {
	return new PlayerActivationCondition();
};