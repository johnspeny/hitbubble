#include "ReviveGameLayer.h"
#include "Constants.h"
#include "scene/GameScene.h"
#include "Audio.h"

USING_NS_AX;
using namespace ui;

void ReviveGameLayer::showGameOverLayer()
{
	retryButton->unschedule("countdown_key");
	retryButton->setTitleText("0");  // Reset the label

	m_gameScene->_isGameOver = true;
	this->removeFromParentAndCleanup(true);
}

ReviveGameLayer::ReviveGameLayer()
{
	m_origin = Director::getInstance()->getVisibleOrigin();
	m_visibleSize = Director::getInstance()->getVisibleSize();
	currentCountdown = countdownSeconds;
}

ReviveGameLayer* ReviveGameLayer::create() {
	auto pRet = new ReviveGameLayer();
	if (pRet->init()) {
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

bool ReviveGameLayer::init() {
	if (!Layer::init())
		return false;

	// play bounce sound
	// Audio::getInstance().playSfx(std::string(sound_audio::sad_crowd));

	auto bgLayer = LayerColor::create(Color4B(0, 0, 0, 200));
	this->addChild(bgLayer);

	// overlay
	overlayLayer = Layer::create();
	this->addChild(overlayLayer);

	//setVisible(true);

	// make black
	//this->setOpacity(0);

	//this->runAction(FadeTo::create(0.15f, 200));

	// bg
	bg = ax::ui::ImageView::create("victory.png", Widget::TextureResType::LOCAL);
	bg->setPosition(Vec2(m_visibleSize.width / 2.0f, m_visibleSize.height * 0.5f));
	bg->setScale(m_visibleSize.width * 0.75f / bg->getContentSize().width, m_visibleSize.height / bg->getContentSize().height / 2);
	bg->setScale9Enabled(true);
	bg->setOpacity(0);
	overlayLayer->addChild(bg, -1);

	// retry button
	retryButton = ui::Button::create("CloseNormal.png", "", "");
	retryButton->setScale9Enabled(true);
	retryButton->setPosition(Vec2(bg->getContentSize().width / 2.0f, bg->getContentSize().height / 2.0f));
	bg->addChild(retryButton);
	retryButton->setTitleText(StringUtils::format("%d", currentCountdown));
	retryButton->setTitleFontSize(30); // Adjust the size as needed
	retryButton->setTitleColor(Color3B::BLUE);  // Set text color to blue
	retryButton->addClickEventListener([=, this](Ref* sender) {
		// Reset flags
		m_gameScene->_isReviveGame = false;
		m_gameScene->_isReviveGameHandled = true;

		//
		m_gameScene->reviveGame();

		retryButton->unschedule("countdown_key");
		this->removeFromParentAndCleanup(true);
		});

	// Schedule a function to update the countdown label
	retryButton->schedule([=](float dt) {
		// Update the countdown label on the button
		retryButton->setTitleText(StringUtils::format("%d", currentCountdown));

		// Check if the countdown has reached zero
		if (currentCountdown <= 0) {
			// Trigger game over
			showGameOverLayer();
		}

		currentCountdown--;
		}, 1.0f, countdownSeconds, 0.0f, "countdown_key");

	// Create skip button
	auto skipButton = ui::Button::create("CloseNormal.png", "", "");
	skipButton->setScale9Enabled(true);
	skipButton->setPosition(Vec2(bg->getContentSize().width / 2.0f, -skipButton->getContentSize().height));
	bg->addChild(skipButton);

	skipButton->setOpacity(0);
	auto fadeIn = FadeIn::create(0.5f);
	auto scaleUp = ScaleTo::create(0.5f, 1.0f);

	ax::CallFunc* pressButton = ax::CallFunc::create([this, skipButton]() {
		// Add a click event listener to the button
		skipButton->addClickEventListener([=, this](Ref* sender) {
			if (this->isVisible()) {
				this->setVisible(false); // Hide the layer when the button is clicked
				// start playing the game
				if (m_gameScene) {
					//
					//Audio::getInstance().stopBgm();

					// reset game
					// m_gameScene->resetGame();

					//m_gameScene->_isGameOver = true;
					//this->removeFromParentAndCleanup(true);

					showGameOverLayer();
				}
			}
			});
		});

	auto sequence = Sequence::create(DelayTime::create(1.5f),
		Spawn::create(fadeIn, scaleUp, nullptr), pressButton, nullptr);
	skipButton->runAction(sequence);

	// actions of bg
	auto fadeInBg = FadeIn::create(0.5f);
	ax::CallFunc* showItsChildren = ax::CallFunc::create([this]() {
		// level
		});
	auto sequenceBg = Sequence::create(DelayTime::create(1.0f),
		fadeInBg, showItsChildren, nullptr);
	bg->runAction(sequenceBg);

	// swallow touches
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);  // Initialize with default value
	listener->onTouchBegan = AX_CALLBACK_2(ReviveGameLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void ReviveGameLayer::setGameSceneReference(GameScene* gameScene) {
	m_gameScene = gameScene;
}

bool ReviveGameLayer::onTouchBegan(ax::Touch* touch, ax::Event* event) {
	// If the game has started, let the touches propagate through
	return true;
}