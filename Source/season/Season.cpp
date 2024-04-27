#include "Season.h"

Season::Season(const std::string& seasonName) : m_name(seasonName) {}

void Season::addLevel(std::unique_ptr<Level> lvl)
{
    m_levels.push_back(std::move(lvl));
}

std::string Season::getName() const
{
    return m_name;
}

const std::vector<std::unique_ptr<Level>>& Season::getLevels() const
{
    return m_levels;
}
