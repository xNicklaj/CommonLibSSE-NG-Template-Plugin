#pragma once

#include "Conditions/Condition.h"
#include <unordered_map>
#include <vector>

class ConditionManager :
	public RE::BSTEventSink<RE::TESQuestStageEvent>,
	public RE::BSTEventSink<RE::LevelIncrease::Event>,
	public RE::BSTEventSink<RE::SkillIncrease::Event>,
	public RE::BSTEventSink<RE::ItemCrafted::Event>,
	public RE::BSTEventSink<RE::LocationDiscovery::Event>,
	public RE::BSTEventSink<RE::TESContainerChangedEvent>,
	public RE::BSTEventSink<RE::SpellsLearned::Event>,
	public RE::BSTEventSink<RE::PositionPlayerEvent>,
	public RE::BSTEventSink<RE::BGSActorCellEvent>,
	public RE::BSTEventSink<RE::TESActivateEvent>,
	public RE::BSTEventSink<RE::DragonSoulsGained::Event>,
	public RE::BSTEventSink<RE::BooksRead::Event>,
	public RE::BSTEventSink<RE::TESDeathEvent>,
	public RE::BSTEventSink<RE::TESTrackedStatsEvent>,
	public RE::BSTEventSink<RE::MenuOpenCloseEvent>
{
public:
	static ConditionManager* GetSingleton()
	{
		static ConditionManager singleton;
		return &singleton;
	}

	void RegisterQuestListener(RE::FormID formID, Condition* condition);
	void RegisterLevelListener(Condition* condition);
	void RegisterSkillListener(Condition* condition);
	void RegisterItemCraftedListener(RE::FormID itemID, Condition* condition);
	void RegisterLocationDiscoveryListener(Condition* condition); // generic bucket due to complexity
	void RegisterItemInInventoryListener(RE::FormID itemID, Condition* condition);
	void RegisterSpellLearnedListener(RE::FormID spellID, Condition* condition);
	void RegisterPositionPlayerListener(Condition* condition);
	void RegisterActorCellListener(RE::FormID cellID, Condition* condition);
	void RegisterActivateListener(RE::FormID formID, Condition* condition);
	void RegisterDragonSoulListener(Condition* condition);
	void RegisterBookReadListener(RE::FormID bookID, Condition* condition);
	void RegisterDeathListener(RE::FormID actorID, Condition* condition);
	void RegisterDeathGenericListener(Condition* condition); // For editorID deaths
	void RegisterTrackedStatListener(std::string statName, Condition* condition);
	void RegisterMenuListener(Condition* condition);

	void EnableListeners();
	void EvaluateTrackedStats();

protected:
	RE::BSEventNotifyControl ProcessEvent(const RE::TESQuestStageEvent* a_event, RE::BSTEventSource<RE::TESQuestStageEvent>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::LevelIncrease::Event* a_event, RE::BSTEventSource<RE::LevelIncrease::Event>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::SkillIncrease::Event* a_event, RE::BSTEventSource<RE::SkillIncrease::Event>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::ItemCrafted::Event* a_event, RE::BSTEventSource<RE::ItemCrafted::Event>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::LocationDiscovery::Event* a_event, RE::BSTEventSource<RE::LocationDiscovery::Event>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::TESContainerChangedEvent* a_event, RE::BSTEventSource<RE::TESContainerChangedEvent>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::SpellsLearned::Event* a_event, RE::BSTEventSource<RE::SpellsLearned::Event>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::PositionPlayerEvent* a_event, RE::BSTEventSource<RE::PositionPlayerEvent>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::BGSActorCellEvent* a_event, RE::BSTEventSource<RE::BGSActorCellEvent>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::DragonSoulsGained::Event* a_event, RE::BSTEventSource<RE::DragonSoulsGained::Event>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::BooksRead::Event* a_event, RE::BSTEventSource<RE::BooksRead::Event>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::TESDeathEvent* a_event, RE::BSTEventSource<RE::TESDeathEvent>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::TESTrackedStatsEvent* a_event, RE::BSTEventSource<RE::TESTrackedStatsEvent>* a_eventSource) override;
	RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

private:
	ConditionManager() = default;
	ConditionManager(const ConditionManager&) = delete;
	ConditionManager& operator=(const ConditionManager&) = delete;

	std::unordered_map<RE::FormID, std::vector<Condition*>> questListeners;
	std::vector<Condition*> levelListeners;
	std::vector<Condition*> skillListeners;
	std::unordered_map<RE::FormID, std::vector<Condition*>> itemCraftedListeners;
	std::vector<Condition*> locationDiscoveryListeners;
	std::unordered_map<RE::FormID, std::vector<Condition*>> itemInInventoryListeners;
	std::unordered_map<RE::FormID, std::vector<Condition*>> spellLearnedListeners;
	std::vector<Condition*> positionPlayerListeners;
	std::unordered_map<RE::FormID, std::vector<Condition*>> actorCellListeners;
	std::unordered_map<RE::FormID, std::vector<Condition*>> activateListeners;
	std::vector<Condition*> dragonSoulListeners;
	std::unordered_map<RE::FormID, std::vector<Condition*>> bookReadListeners;
	std::unordered_map<RE::FormID, std::vector<Condition*>> deathListeners;
	std::vector<Condition*> deathGenericListeners;
	std::unordered_map<std::string, std::vector<Condition*>> trackedStatListeners;
	std::vector<Condition*> menuListeners;

	// Used to run condition checks dynamically
	void DispatchToBucket(std::vector<Condition*>& bucket);
	void DispatchToBucketFormID(RE::FormID formID, std::unordered_map<RE::FormID, std::vector<Condition*>>& map);
};
