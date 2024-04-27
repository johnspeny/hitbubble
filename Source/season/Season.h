#pragma once

#include "axmol.h"
#include <vector>
#include <string>

struct Level
{
    int id;
    std::string name;
    bool completed;
    bool unlocked;
    int points;
    bool pointsAwarded;
};

class Season
{
private:
    std::string m_name;
    std::vector<std::unique_ptr<Level>> m_levels;

public:
    Season(const std::string& seasonName);
    void addLevel(std::unique_ptr<Level> lvl);
    std::string getName() const;
    const std::vector<std::unique_ptr<Level>>& getLevels() const;
};
