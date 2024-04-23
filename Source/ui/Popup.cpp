#include "Popup.h"
#include "Constants.h"
#include "scene/LevelSelectScene.h"
#include "scene/MainMenuScene.h"
#include "scene/GameScene.h"
#include "scene/ShopScene.h"
#include "AudioEngine.h"

using namespace ax;
using namespace ui;

namespace
{
	const float ANIMATION_TIME = 0.15f;
	unsigned char FADE_RATIO = 200;
}

namespace UICustom {
	Popup::Popup(GameScene* gameScene) :
		m_bg{},
		m_isAnimated{ false },
		m_gameScene{ gameScene }
	{
		// get audio
		m_audioID = m_audioID = UserDefault::getInstance()->getIntegerForKey("AudioID", 1);
	}

	Popup* Popup::create(GameScene* gameScene)
	{
		auto* node = new (std::nothrow)Popup(gameScene);
		if (node->init())
		{
			node->autorelease();
			return node;
		}
		AX_SAFE_DELETE(node);
		return nullptr;
	}

	bool Popup::init()
	{
		auto winSize = Director::getInstance()->getVisibleSize();

		if (!LayerColor::initWithColor(Color4B(0, 0, 0, FADE_RATIO)))
		{
			return false;
		}

		this->setOpacity(0);

		show(true);

		initBg();

		setUpTouches();

		return true;
	}

	void Popup::show(bool animated)
	{
		if (animated) {
			this->runAction(FadeTo::create(ANIMATION_TIME, FADE_RATIO));
		}
		else {
			this->setOpacity(FADE_RATIO);
		}
	}

	void Popup::dismiss(const bool animated)
	{
		if (animated) {
			Director::getInstance()->resume();
			m_gameScene->isUpdatePaused = false;
			this->runAction(Sequence::create(FadeTo::create(ANIMATION_TIME, 0), RemoveSelf::create(), NULL));
		}
		else {
			this->removeFromParentAndCleanup(true);
		}
	}

	void Popup::initBg()
	{
		auto winSize = Director::getInstance()->getVisibleSize();
		auto origin = Director::getInstance()->getVisibleOrigin();

		// first bg image
		m_bg = ui::ImageView::create("star_button_bg.png", Widget::TextureResType::PLIST);
		m_bg->setPosition(Vec2(origin.x + winSize.width / 2.0f, origin.y + winSize.height / 2.0f));
		m_bg->setScale9Enabled(true);
		m_bg->setContentSize({ winSize.width * 0.8f, winSize.height * 0.2f });
		this->addChild(m_bg);

		// title
		auto heading = Label::createWithBMFont(fonts::hugmat_fnt, "PAUSE");
		heading->setBMFontSize(24);
		heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		heading->setPosition(m_bg->getContentSize().width / 2, m_bg->getContentSize().height - heading->getContentSize().height);
		m_bg->addChild(heading);

		// top right button for closing
		auto closeBtn = Button::create("xmark.png",
			"xmark.png",
			"xmark.png",
			Widget::TextureResType::PLIST);
		closeBtn->setScale(0.5f);
		closeBtn->setPosition({ m_bg->getContentSize().width, m_bg->getContentSize().height });
		closeBtn->addTouchEventListener(
			[&](Ref* sender, ax::ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case ui::Widget::TouchEventType::BEGAN:
					break;
				case ui::Widget::TouchEventType::ENDED:
					// play button click sound
					if (m_audioID != AudioEngine::INVALID_AUDIO_ID)
					{
						AudioEngine::play2d(sound_audio::btn_click, false);
						//AudioEngine::stopAll();
					}
					this->dismiss(true);
					break;
				default:
					break;
				}
			}
		);
		m_bg->addChild(closeBtn);

		// level, Home, shop
		SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();

		auto levelBtn = MenuItemImage::create();
		levelBtn->setNormalSpriteFrame(frameCache->getSpriteFrameByName("list_button.png"));
		levelBtn->setSelectedSpriteFrame(frameCache->getSpriteFrameByName("list_button.png"));
		levelBtn->setScale(0.5f);
		levelBtn->setCallback([&](Ref* sender) {
			// play button click sound
			if (m_audioID != AudioEngine::INVALID_AUDIO_ID)
			{
				AudioEngine::play2d(sound_audio::btn_click, false);
				//AudioEngine::stopAll();
			}
			auto levelScene = LevelSelectScene::create();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, levelScene, Color3B::WHITE));
			});

		auto homeBtn = MenuItemImage::create();
		homeBtn->setNormalSpriteFrame(frameCache->getSpriteFrameByName("home.png"));
		homeBtn->setSelectedSpriteFrame(frameCache->getSpriteFrameByName("home.png"));
		homeBtn->setScale(0.5f);
		homeBtn->setCallback([&](Ref* sender) {
			// play button click sound
			if (m_audioID != AudioEngine::INVALID_AUDIO_ID)
			{
				AudioEngine::play2d(sound_audio::btn_click, false);
				//AudioEngine::stopAll();
			}
			auto mainScene = MainMenuScene::create();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, mainScene, Color3B::WHITE));
			});

		auto shopBtn = MenuItemImage::create();
		shopBtn->setNormalSpriteFrame(frameCache->getSpriteFrameByName("shop_basket.png"));
		shopBtn->setSelectedSpriteFrame(frameCache->getSpriteFrameByName("shop_basket.png"));
		shopBtn->setScale(0.5f);
		shopBtn->setCallback([&](Ref* sender) {
			// play button click sound
			if (m_audioID != AudioEngine::INVALID_AUDIO_ID)
			{
				AudioEngine::play2d(sound_audio::btn_click, false);
				//AudioEngine::stopAll();
			}
			auto scene = ShopScene::create();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene, Color3B::WHITE));
			});

		auto menu = Menu::create(levelBtn, homeBtn, shopBtn, NULL);
		menu->setPosition(m_bg->getContentSize().width / 2, m_bg->getContentSize().height * 0.5f);
		menu->alignItemsHorizontallyWithPadding(10);
		m_bg->addChild(menu, 2);
	}

	void Popup::setUpTouches()
	{
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = [=](Touch* touch, Event* event)->bool
		{
			return true;
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}
}