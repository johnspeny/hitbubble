#include "ToolBar.h"

ToolBar::ToolBar() :
	m_leftItem{},
	m_rightItem{},
	m_middleItem{}
{
	m_origin = Director::getInstance()->getVisibleOrigin();
	m_visibleSize = Director::getInstance()->getVisibleSize();
}

ToolBar* ToolBar::create()
{
	auto pRet = new ToolBar();
	if (pRet->init())
		pRet->autorelease();
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

bool ToolBar::init()
{
	if (!ax::ui::Layout::init()) return false;

	// define the layout structure
	// Create a container layout for the flow layout
	layoutStructure();

	return true;
}

void ToolBar::addLeftComponent(Widget* component, float scale)
{
	m_leftItem = component;
	m_leftItem->setPositionX(m_leftItem->getContentSize().width * scale);
	m_leftItem->setPositionY(this->getContentSize().height * 0.5f);
	this->addChild(m_leftItem);
}

void ToolBar::addRightComponent(Widget* component, float scale)
{
	m_rightItem = component;
	m_rightItem->setPositionX(this->getContentSize().width - m_rightItem->getContentSize().width * scale);
	m_rightItem->setPositionY(this->getContentSize().height * 0.5f);
	this->addChild(m_rightItem);
}

void ToolBar::addMiddleComponent(Widget* component, float scale)
{
	m_middleItem = component;
	// Set the content size of the middle button
	// remaining width is used to set context size
	float padding = 50.0f;
	float middleWidth = this->getContentSize().width - m_leftItem->getContentSize().width * scale - m_rightItem->getContentSize().width * scale - (padding * 2);
	m_middleItem->setContentSize(Size(middleWidth, this->getContentSize().height));

	m_middleItem->setPositionX(this->getContentSize().width * 0.5f);
	m_middleItem->setPositionY(this->getContentSize().height * 0.5f);

	this->addChild(m_middleItem);
}

void ToolBar::addComponentAtCenter(Widget* component)
{
	component->setPosition({ this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f });
	this->addChild(component);
}

void ToolBar::layoutStructure()
{
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setContentSize(Size(m_visibleSize.width, kLayoutHeight));
	this->setPosition(Vec2(m_origin.x + m_visibleSize.width / 2.0f, m_visibleSize.height - this->getContentSize().height * 0.5f));
	this->setLayoutType(Layout::Type::ABSOLUTE);
	//this->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	//this->setBackGroundColor(Color3B::BLUE);
}

void ToolBar::layoutStructure(const Vec2& pos, const Color3B& color)
{
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setContentSize(Size(m_visibleSize.width, kLayoutHeight));
	this->setPosition(pos);
	this->setLayoutType(Layout::Type::ABSOLUTE);
	//this->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	//this->setBackGroundColor(color);
}