#include "ConditionManager.h"
#include "log.h"

void ConditionManager::RegisterQuestListener(RE::FormID formID, Condition* condition) {
	questListeners[formID].push_back(condition);
}
void ConditionManager::RegisterLevelListener(Condition* condition) {
	levelListeners.push_back(condition);
}
void ConditionManager::RegisterSkillListener(Condition* condition) {
	skillListeners.push_back(condition);
}
void ConditionManager::RegisterItemCraftedListener(RE::FormID itemID, Condition* condition) {
	itemCraftedListeners[itemID].push_back(condition);
}
void ConditionManager::RegisterLocationDiscoveryListener(Condition* condition) {
	locationDiscoveryListeners.push_back(condition);
}
void ConditionManager::RegisterItemInInventoryListener(RE::FormID itemID, Condition* condition) {
	itemInInventoryListeners[itemID].push_back(condition);
}
void ConditionManager::RegisterSpellLearnedListener(RE::FormID spellID, Condition* condition) {
	spellLearnedListeners[spellID].push_back(condition);
}
void ConditionManager::RegisterPositionPlayerListener(Condition* condition) {
	positionPlayerListeners.push_back(condition);
}
void ConditionManager::RegisterActorCellListener(RE::FormID cellID, Condition* condition) {
	actorCellListeners[cellID].push_back(condition);
}
void ConditionManager::RegisterActivateListener(RE::FormID formID, Condition* condition) {
	activateListeners[formID].push_back(condition);
}
void ConditionManager::RegisterDragonSoulListener(Condition* condition) {
	dragonSoulListeners.push_back(condition);
}
void ConditionManager::RegisterBookReadListener(RE::FormID bookID, Condition* condition) {
	bookReadListeners[bookID].push_back(condition);
}
void ConditionManager::RegisterDeathListener(RE::FormID actorID, Condition* condition) {
	deathListeners[actorID].push_back(condition);
}
void ConditionManager::RegisterDeathGenericListener(Condition* condition) {
	deathGenericListeners.push_back(condition);
}
void ConditionManager::RegisterTrackedStatListener(std::string statName, Condition* condition) {
	trackedStatListeners[statName].push_back(condition);
}
void ConditionManager::RegisterMenuListener(Condition* condition) {
	menuListeners.push_back(condition);
}

void ConditionManager::EnableListeners() {
	auto* scriptEventSource = RE::ScriptEventSourceHolder::GetSingleton();
	if (scriptEventSource) {
		if (!questListeners.empty()) scriptEventSource->AddEventSink<RE::TESQuestStageEvent>(this);
		if (!itemCraftedListeners.empty()) RE::ItemCrafted::GetEventSource()->AddEventSink(this);
		if (!deathListeners.empty() || !deathGenericListeners.empty()) scriptEventSource->AddEventSink<RE::TESDeathEvent>(this);
		if (!itemInInventoryListeners.empty()) scriptEventSource->AddEventSink<RE::TESContainerChangedEvent>(this);
		if (!trackedStatListeners.empty()) scriptEventSource->AddEventSink<RE::TESTrackedStatsEvent>(this);
	}
	
	if (!menuListeners.empty()) RE::UI::GetSingleton()->AddEventSink<RE::MenuOpenCloseEvent>(this);
	if (!levelListeners.empty()) RE::LevelIncrease::GetEventSource()->AddEventSink(this);
	if (!skillListeners.empty()) RE::SkillIncrease::GetEventSource()->AddEventSink(this);
	if (!locationDiscoveryListeners.empty()) RE::LocationDiscovery::GetEventSource()->AddEventSink(this);
	if (!spellLearnedListeners.empty()) RE::SpellsLearned::GetEventSource()->AddEventSink(this);
	if (!positionPlayerListeners.empty()) RE::PlayerCharacter::GetSingleton()->AsPositionPlayerEventSource()->AddEventSink<RE::PositionPlayerEvent>(this);
	if (!actorCellListeners.empty()) RE::PlayerCharacter::GetSingleton()->AsBGSActorCellEventSource()->AddEventSink(this);
	if (!activateListeners.empty()) scriptEventSource->AddEventSink<RE::TESActivateEvent>(this);
	if (!dragonSoulListeners.empty()) RE::DragonSoulsGained::GetEventSource()->AddEventSink(this);
	if (!bookReadListeners.empty()) RE::BooksRead::GetEventSource()->AddEventSink(this);
}

void ConditionManager::DispatchToBucket(std::vector<Condition*>& bucket) {
	for (auto it = bucket.begin(); it != bucket.end();) {
		Condition* cond = *it;
		if (cond->CheckCondition()) {
			it = bucket.erase(it);
		} else {
			++it;
		}
	}
}

void ConditionManager::EvaluateTrackedStats() {
	for (auto it = trackedStatListeners.begin(); it != trackedStatListeners.end();) {
		DispatchToBucket(it->second);
		if (it->second.empty()) {
			it = trackedStatListeners.erase(it);
		} else {
			++it;
		}
	}
}

void ConditionManager::DispatchToBucketFormID(RE::FormID formID, std::unordered_map<RE::FormID, std::vector<Condition*>>& map) {
	auto itMap = map.find(formID);
	if (itMap != map.end()) {
		auto& bucket = itMap->second;
		for (auto it = bucket.begin(); it != bucket.end();) {
			Condition* cond = *it;
			if (cond->CheckCondition()) {
				it = bucket.erase(it);
			} else {
				++it;
			}
		}
		if (bucket.empty()) {
			map.erase(itMap);
		}
	}
}

RE::BSEventNotifyControl ConditionManager::ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>*) {
	if (a_event) DispatchToBucketFormID(a_event->formID, questListeners);
	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ConditionManager::ProcessEvent(const RE::LevelIncrease::Event* a_event, RE::BSTEventSource<RE::LevelIncrease::Event>*) {
	if (a_event) DispatchToBucket(levelListeners);
	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ConditionManager::ProcessEvent(const RE::SkillIncrease::Event* a_event, RE::BSTEventSource<RE::SkillIncrease::Event>*) {
	if (a_event) DispatchToBucket(skillListeners);
	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ConditionManager::ProcessEvent(const RE::ItemCrafted::Event* a_event, RE::BSTEventSource<RE::ItemCrafted::Event>*) {
	if (a_event && a_event->item) DispatchToBucketFormID(a_event->item->formID, itemCraftedListeners);
	EvaluateTrackedStats();
	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ConditionManager::ProcessEvent(const RE::LocationDiscovery::Event* a_event, RE::BSTEventSource<RE::LocationDiscovery::Event>*) {
	if (a_event) {
		for (auto it = locationDiscoveryListeners.begin(); it != locationDiscoveryListeners.end();) {
			Condition* cond = *it;
			cond->OnLocationDiscoveryEvent(a_event);
			if (cond->isMet) {
				it = locationDiscoveryListeners.erase(it);
			} else {
				++it;
			}
		}
	}
	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ConditionManager::ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {
	if (a_event && a_event->newContainer == RE::PlayerCharacter::GetSingleton()->GetFormID()) {
		DispatchToBucketFormID(a_event->baseObj, itemInInventoryListeners);
	}
	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ConditionManager::ProcessEvent(const RE::SpellsLearned::Event* a_event, RE::BSTEventSource<RE::SpellsLearned::Event>*) {
	if (a_event && a_event->spell) DispatchToBucketFormID(a_event->spell->GetFormID(), spellLearnedListeners);
	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ConditionManager::ProcessEvent(const RE::PositionPlayerEvent* a_event, RE::BSTEventSource<RE::PositionPlayerEvent>*) {
	if (a_event) DispatchToBucket(positionPlayerListeners);
	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ConditionManager::ProcessEvent(const RE::BGSActorCellEvent* a_event, RE::BSTEventSource<RE::BGSActorCellEvent>*) {
	if (a_event && a_event->cellID && a_event->flags == RE::BGSActorCellEvent::CellFlag::kEnter) {
		DispatchToBucketFormID(a_event->cellID, actorCellListeners);
	}
	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ConditionManager::ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>*) {
	if (a_event && a_event->objectActivated) {
		DispatchToBucketFormID(a_event->objectActivated->GetFormID(), activateListeners);
	}
	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ConditionManager::ProcessEvent(const RE::DragonSoulsGained::Event* a_event, RE::BSTEventSource<RE::DragonSoulsGained::Event>*) {
	if (a_event) DispatchToBucket(dragonSoulListeners);
	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ConditionManager::ProcessEvent(const RE::BooksRead::Event* a_event, RE::BSTEventSource<RE::BooksRead::Event>*) {
	if (a_event && a_event->book) {
		DispatchToBucketFormID(a_event->book->formID, bookReadListeners);
		
		// Generic book reads (by title) are registered at formID 0
		auto itMap = bookReadListeners.find(0);
		if (itMap != bookReadListeners.end()) {
			auto& bucket = itMap->second;
			for (auto it = bucket.begin(); it != bucket.end();) {
				Condition* cond = *it;
				cond->OnBookReadEvent(a_event);
				if (cond->isMet) {
					it = bucket.erase(it);
				} else {
					++it;
				}
			}
			if (bucket.empty()) bookReadListeners.erase(itMap);
		}
	}
	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ConditionManager::ProcessEvent(const RE::TESDeathEvent* a_event, RE::BSTEventSource<RE::TESDeathEvent>*) {
	if (a_event && a_event->actorDying && a_event->dead) {
		// Specific FormID deaths
		auto objRefr = a_event->actorDying->data.objectReference;
		if (objRefr && objRefr->As<RE::TESNPC>()) {
			DispatchToBucketFormID(objRefr->As<RE::TESNPC>()->GetFormID(), deathListeners);
		}
		
		// Generic deaths (e.g. check by EditorID or Name)
		for (auto it = deathGenericListeners.begin(); it != deathGenericListeners.end();) {
			Condition* cond = *it;
			cond->OnDeathEvent(a_event);
			if (cond->isMet) {
				it = deathGenericListeners.erase(it);
			} else {
				++it;
			}
		}
	}
	EvaluateTrackedStats();
	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ConditionManager::ProcessEvent(const RE::TESTrackedStatsEvent* a_event, RE::BSTEventSource<RE::TESTrackedStatsEvent>*) {
	if (a_event) {
		logger::debug("Tracked stat event fired for stat: '{}'", a_event->stat.c_str());
		for (auto it = trackedStatListeners.begin(); it != trackedStatListeners.end();) {
			if (_stricmp(it->first.c_str(), a_event->stat.c_str()) == 0) {
				DispatchToBucket(it->second);
				if (it->second.empty()) {
					it = trackedStatListeners.erase(it);
					continue;
				}
			}
			++it;
		}
	}
	return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ConditionManager::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) {
	if (a_event) DispatchToBucket(menuListeners);
	EvaluateTrackedStats();
	return RE::BSEventNotifyControl::kContinue;
}
