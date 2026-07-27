#include "../Condition.h"

#ifndef OBJECT_ENABLED_CONDITION_H
#define OBJECT_ENABLED_CONDITION_H

class ObjectStateCondition : public Condition {
public:
    ObjectStateCondition();
    void OnDataLoaded() override;
    void EnableListener() override;
    void SetConditionParameters(std::string formID, bool state) override;
    bool CheckCondition();

	RE::TESObjectREFR* cachedRef = nullptr;
private:
    std::string formID = "";
    bool state = false;
    
};

class ObjectStateConditionFactory : public ConditionFactory {
    public:
        ObjectStateConditionFactory();
        Condition* createCondition() override;
};

#endif