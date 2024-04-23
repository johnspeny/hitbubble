#include "MainMenuLayer.h"
#include "Constants.h"
#include "scene/GameScene.h"

USING_NS_AX;
using namespace ui;

MainMenuLayer::MainMenuLayer() :
	m_touchBgImage{}
{
	m_origin = Director::getInstance()->getVisibleOrigin();
	m_visibleSize = Director::getInstance()->getVisibleSize();
}

MainMenuLayer* MainMenuLayer::create()
{
	auto pRet = new MainMenuLayer();
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

bool MainMenuLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(255, 0, 0, 0)))
		return false;

	// Create a button -------------------------------------------------------------------------
	auto playButton = ui::Button::create("transparentDark40.png", "", "");
	playButton->setPosition(Vec2(m_visibleSize.width / 2.0f, m_visibleSize.height * 0.5f));
	playButton->setScale9Enabled(true);
	//playButton->setScale(0.5f);
	playButton->setSwallowTouches(true);
	addChild(playButton);

	auto swipe_hand = Sprite::create("swipehand.png");
	swipe_hand->setScale(0.5f);
	swipe_hand->setPosition(Vec2(m_visibleSize.width / 2.0f, m_visibleSize.height * 0.16f));
	addChild(swipe_hand);

	auto swipe_tut = Sprite::create("swipe_tut.png");
	swipe_tut->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	swipe_tut->setPosition(Vec2(swipe_hand->getContentSize().width, 0));
	swipe_hand->addChild(swipe_tut);

	//actions
	float duration = 2.0f; // Adjust the duration as needed

	//auto moveToRight = MoveTo::create(duration, Vec2(m_visibleSize.width * 0.8f, m_visibleSize.height * 0.16f));
	//auto moveToLeft = MoveTo::create(duration, Vec2(m_visibleSize.width * 0.2f, m_visibleSize.height * 0.16f));

	auto moveToRight = EaseInOut::create(MoveTo::create(duration, Vec2(m_visibleSize.width * 0.8f, m_visibleSize.height * 0.16f)), 2.0f);
	auto moveToLeft = EaseInOut::create(MoveTo::create(duration, Vec2(m_visibleSize.width * 0.2f, m_visibleSize.height * 0.16f)), 2.0f);

	auto sequence = Sequence::create(moveToRight, moveToLeft, nullptr);
	auto repeatForever = RepeatForever::create(sequence);

	swipe_hand->runAction(repeatForever);

	// Add a click event listener to the button
	playButton->addClickEventListener([=](Ref* sender) {
		if (this->isVisible()) {
			this->setVisible(false); // Hide the layer when the button is clicked
			// start playing the game
			if (m_gameScene)
			{
				m_gameScene->playGame();
				m_gameScene->_isStartGame = true;
				updateSwallowTouches();
				// Remove the MainMenuLayer from its parent
				this->removeFromParentAndCleanup(true);
			}
		}
		});

	// swallow touches
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(m_swallowTouches);  // Initialize with default value
	listener->onTouchBegan = AX_CALLBACK_2(MainMenuLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	m_touchListener = listener;  // Store the listener

	return true;
}

void  MainMenuLayer::updateSwallowTouches()
{
	// Set m_swallowTouches based on the game state
	m_swallowTouches = !m_gameScene || !m_gameScene->_isStartGame;

	// Set the state for the listener
	if (m_touchListener) {
		m_touchListener->setSwallowTouches(m_swallowTouches);
	}
}

void MainMenuLayer::setGameSceneReference(GameScene* gameScene)
{
	m_gameScene = gameScene;
	updateSwallowTouches();
}

bool MainMenuLayer::onTouchBegan(ax::Touch* touch, ax::Event* event)
{
	if (!m_gameScene->_isStartGame) {
		// If the game hasn't started yet, swallow the touches
		return true;
	}

	// If the game has started, let the touches propagate through
	return false;
}