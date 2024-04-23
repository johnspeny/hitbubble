#include "BottomNavigationBar.h"

BottomNavigationBar::BottomNavigationBar()
{
}

BottomNavigationBar* BottomNavigationBar::create()
{
	auto pRet = new BottomNavigationBar();
	if (pRet->init())
		pRet->autorelease();
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

bool BottomNavigationBar::init()
{
	if (!ax::ui::Layout::init()) return false;

	// define the layout structure
	// Create a container layout for the flow layout
	layoutStructure();

	return true;
}

void BottomNavigationBar::layoutStructure()
{
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setContentSize(Size(m_visibleSize.width, kLayoutHeight));
	this->setPosition(Vec2(m_origin.x + m_visibleSize.width / 2.0f, this->getContentSize().height + this->getContentSize().height/2.0f));
	this->setLayoutType(Layout::Type::ABSOLUTE);
	//this->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	//this->setBackGroundColor(Color3B::YELLOW);
}