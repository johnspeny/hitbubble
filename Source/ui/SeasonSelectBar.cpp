#include "SeasonSelectBar.h"

SeasonSelectBar::SeasonSelectBar()
{}

SeasonSelectBar* SeasonSelectBar::create()
{
	auto pRet = new SeasonSelectBar();
	if (pRet->init())
		pRet->autorelease();
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

bool SeasonSelectBar::init()
{
	if (!ax::ui::Layout::init()) return false;

	// define the layout structure
	// Create a container layout for the flow layout
	layoutStructure();

	return true;
}

void SeasonSelectBar::layoutStructure()
{
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setContentSize(Size(m_visibleSize.width, kLayoutHeight));
	this->setPosition(Vec2(m_origin.x + m_visibleSize.width / 2.0f, m_visibleSize.height - this->getContentSize().height * 0.5f));
	this->setLayoutType(Layout::Type::ABSOLUTE);
	//this->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	//this->setBackGroundColor(Color3B::BLUE);
}