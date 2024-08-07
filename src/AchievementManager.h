#include "Singleton.h"
#include <nlohmann/json.hpp>
#include "Achievement.h"

using json = nlohmann::json;

struct AchievementFile {
	std::string groupName;
	std::string plugin;
	json FileData;
};

struct AchievementGroup
{
	std::string name;
	std::string plugin;
	std::vector<Achievement*> achievements;

	AchievementGroup(std::string name, std::string plugin) : name(name), plugin(plugin) {}
};

class AchievementManager : public ISingleton<AchievementManager> {
public:
	std::vector<AchievementFile> achievementFiles;
	std::vector<AchievementGroup> achievementGroups;
};