#include "TopContainer.h"
#include "Constants.h"

USING_NS_AX;
using namespace ax::ui;

TopContainer::TopContainer()
{
	m_origin = Director::getInstance()->getVisibleOrigin();
	m_visibleSize = Director::getInstance()->getVisibleSize();
}

TopContainer* TopContainer::create()
{
	auto pRet = new TopContainer();
	if (pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

bool TopContainer::init()
{
	if (!ax::ui::Layout::init()) {
		return false;
	}

	// Create a container layout for the flow layout
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setContentSize(Size(m_visibleSize.width, 50));
	this->setPosition(Vec2(m_origin.x + m_visibleSize.width / 2.0f, m_visibleSize.height - this->getContentSize().height * 0.5f));
	this->setLayoutType(Layout::Type::ABSOLUTE);
	//this->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	//this->setBackGroundColor(Color3B::BLUE);

	addHomeButton();
	addLevelDisplay();
	addCoinButton();

	return true;
}

void TopContainer::addHomeButton()
{
	auto homeButton = Button::create("home.png", "home.png", "home.png");
	homeButton->setScale(0.5f);
	homeButton->setPositionX(homeButton->getContentSize().width * homeButton->getScale());
	homeButton->setPositionY(this->getContentSize().height * 0.5f);
	homeButton->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type) {
		auto button = dynamic_cast<Button*>(sender);
		if (!button) return;

		if (type == Widget::TouchEventType::BEGAN) {
			button->setColor(Color3B(150, 150, 150));
		}
		else if (type == Widget::TouchEventType::CANCELED || type == Widget::TouchEventType::ENDED) {
			button->setColor(Color3B::WHITE);
			// Handle button logic
		}
		});
	this->addChild(homeButton);
}

void TopContainer::addCoinButton()
{
	auto coinBtn = Button::create("lollipopFruitYellow.png", "lollipopFruitYellow.png", "lollipopFruitYellow.png");
	coinBtn->setScale9Enabled(true);
	coinBtn->setScale(0.25f);
	coinBtn->setPositionX(this->getContentSize().width - coinBtn->getContentSize().width * coinBtn->getScale());
	coinBtn->setPositionY(this->getContentSize().height * 0.5f);
	this->addChild(coinBtn);
	coinBtnWorldPos = coinBtn->getParent()->convertToWorldSpace(coinBtn->getPosition());;

	coinLabel = Label::createWithBMFont(fonts::eng_cin_fnt, "0");
	coinLabel->setBMFontSize(24.0f);
	coinLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	coinLabel->setPositionX(this->getContentSize().width - coinBtn->getContentSize().width * coinBtn->getScale() - coinBtn->getContentSize().width * 1.0f * coinBtn->getScale());
	coinLabel->setPositionY(this->getContentSize().height * 0.5f);
	this->addChild(coinLabel);
}

void TopContainer::addLevelDisplay()
{
	auto level = "Level 1";
	auto levelLabel = Label::createWithBMFont(fonts::eng_cin_fnt, level);
	levelLabel->setBMFontSize(16.0f);
	levelLabel->setColor(Color3B::BLACK);
	levelLabel->setPositionX(this->getContentSize().width / 2.0f);
	levelLabel->setPositionY(this->getContentSize().height * 0.5f);
	addChild(levelLabel);
}

void TopContainer::updateCoinLabel(int totalCoin)
{
	if (coinLabel) {
		coinLabel->setString(std::to_string(totalCoin));
	}
}