#ifndef QUERY_STAT_VALUE_CONDITION_H
#define QUERY_STAT_VALUE_CONDITION_H

#include "../Condition.h" // Ensure this is the correct path to Condition.h

class QueryStatValueCondition : public Condition {
public:
    QueryStatValueCondition();
    void OnDataLoaded() override;
    void EnableListener() override;
    void SetConditionParameters(std::string stat, float value) override;
    bool CheckCondition();

private:
    std::string stat = "";
    float value = -1;

    
};

class QueryStatValueConditionFactory : public ConditionFactory {
public:
    QueryStatValueConditionFactory();
    Condition* createCondition() override;
};

#endif // QUERY_STAT_VALUE_CONDITION_H
