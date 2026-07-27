#include "BaseActorDeathCondition.h"
#include "../../CommonFunctions.h"
#include "../../ConditionManager.h"


extern void RegisterPostLoadFunction(Condition* condition);

BaseActorDeathCondition::BaseActorDeathCondition() : Condition(ConditionType::BaseActorDeath){};
void BaseActorDeathCondition::OnDataLoaded(void) {
	if (this->isFormID) this->cachedNPC = static_cast<RE::TESNPC*>(GetForm(this->identifier, this->plugin));
	if(this->isFormID && this->cachedNPC) ConditionManager::GetSingleton()->RegisterDeathListener(this->cachedNPC->formID, this); else ConditionManager::GetSingleton()->RegisterDeathGenericListener(this);
}
void BaseActorDeathCondition::EnableListener(void) {
	RegisterPostLoadFunction(this);
	
};
void BaseActorDeathCondition::SetConditionParameters(std::string identifier_a, int quantity_a) {
	this->isFormID = isHex(identifier_a);
	this->identifier = identifier_a;
	this->quantity = quantity_a;
};
void BaseActorDeathCondition::SetEditorID(bool a_isEditorID) {
    this->isEditorID = a_isEditorID;
}
bool BaseActorDeathCondition::CheckCondition() {
	if (this->currQuantity >= this->quantity) {
		logger::debug("Player met condition actor {} dead.", this->identifier);
		this->UnlockNotify();
		return true;
	}
	return false;
}

void BaseActorDeathCondition::OnDeathEvent(const RE::TESDeathEvent* event) {
	if (!event || !event->actorDying || !event->dead) return;
	
	auto objRefr = event->actorDying->data.objectReference;
	if (!objRefr) return;
	
	auto npc = objRefr->As<RE::TESNPC>();
	if (!npc) return;
	
	bool matched = false;
	if (this->isFormID) {
		if (this->cachedNPC && npc->GetFormID() == this->cachedNPC->GetFormID()) {
			matched = true;
		}
	} else if (this->isEditorID) {
		std::string editorID = clib_util::editorID::get_editorID(npc);
		if (editorID == this->identifier) matched = true;
	} else {
		std::string name = npc->GetName();
		if (name == this->identifier) matched = true;
	}
	
	if (matched) {
		this->currQuantity++;
		this->CheckCondition();
	}
}
;

bool BaseActorDeathCondition::Deserialize(int value) {
	this->currQuantity = value;
	return this->currQuantity >= this->quantity;
}

int BaseActorDeathCondition::Serialize() {
	return currQuantity;
}

void BaseActorDeathCondition::Localize(std::string path) {
	if (!this->isFormID && this->identifier[0] == '$')
		this->identifier = LocalizationManager::GetSingleton()->GetLocalizedText(path, LocalizationManager::GetSingleton()->CurrentLocale(), this->identifier);
}

BaseActorDeathConditionFactory::BaseActorDeathConditionFactory() : ConditionFactory() {};
Condition* BaseActorDeathConditionFactory::createCondition() {
	return new BaseActorDeathCondition();
};