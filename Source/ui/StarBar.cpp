#include "StarBar.h"

StarBar::StarBar(const SeasonSelectBar& seasonSeleBar) :
	m_seasonSeleBar{ seasonSeleBar }
{
}

StarBar* StarBar::create(const SeasonSelectBar& seasonSeleBar)
{
	auto pRet = new StarBar(seasonSeleBar);
	if (pRet->init())
		pRet->autorelease();
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

bool StarBar::init()
{
	if (!ToolBar::init()) return false;
	// define the layout structure
	// Create a container layout for the flow layout
	layoutStructure(Vec2(m_origin.x + m_visibleSize.width / 2.0f, m_seasonSeleBar.getPositionY() - m_seasonSeleBar.getContentSize().height), Color3B::GRAY);
	return true;
}