#include "VictoryGameLayer.h"
#include "Constants.h"
#include "scene/GameScene.h"
#include "MainMenuLayer.h"
#include "Audio.h"
#include "particlez/FireWorksParticle.h"

USING_NS_AX;
using namespace ui;

VictoryGameLayer::VictoryGameLayer(const std::string& _mission) : m_mission{_mission}
{
    m_origin      = Director::getInstance()->getVisibleOrigin();
    m_visibleSize = Director::getInstance()->getVisibleSize();
}

VictoryGameLayer* VictoryGameLayer::create(const std::string& _mission)
{
    auto pRet = new VictoryGameLayer(_mission);
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

bool VictoryGameLayer::init()
{
    if (!Layer::init())
        return false;

    // play bounce sound
    Audio::getInstance().playSfx(std::string(sound_audio::applause));

    auto bgLayer = LayerColor::create(Color4B(0, 0, 0, 200));
    this->addChild(bgLayer);

    // overlay
    overlayLayer = Layer::create();
    this->addChild(overlayLayer);

    // bg
    bg = ax::ui::ImageView::create("victory.png", Widget::TextureResType::LOCAL);
    bg->setPosition(Vec2(m_visibleSize.width / 2.0f, m_visibleSize.height * 0.5f));
    bg->setScale(m_visibleSize.width * 0.75f / bg->getContentSize().width,
                 m_visibleSize.height / bg->getContentSize().height / 2);
    bg->setScale9Enabled(true);
    bg->setOpacity(0);
    overlayLayer->addChild(bg, -1);

    // Create a button
    auto retryButton = ui::Button::create("btn_continue.png", "", "");
    retryButton->setScale9Enabled(true);
    retryButton->setPosition(Vec2(bg->getContentSize().width / 2.0f, -retryButton->getContentSize().height));
    bg->addChild(retryButton);

    retryButton->setOpacity(0);
    auto fadeIn  = FadeIn::create(0.5f);
    auto scaleUp = ScaleTo::create(0.5f, 1.0f);

    ax::CallFunc* pressButton = ax::CallFunc::create([this, retryButton]() {
        // Add a click event listener to the button
        retryButton->addClickEventListener([=, this](Ref* sender) {
            if (this->isVisible())
            {
                this->setVisible(false);  // Hide the layer when the button is clicked
                // start playing the game
                if (m_gameScene)
                {
                    //
                    // Audio::getInstance().stopBgm();

                    // reset game
                    m_gameScene->resetGame();
                }
            }
        });
    });

    auto sequence =
        Sequence::create(DelayTime::create(1.5f), Spawn::create(fadeIn, scaleUp, nullptr), pressButton, nullptr);
    retryButton->runAction(sequence);

    // actions of bg
    auto fadeInBg                 = FadeIn::create(0.5f);
    ax::CallFunc* showItsChildren = ax::CallFunc::create([this]() {
        // level
        displayCurrentLevel();
        auto fireworks = FireWorksParticle::create();
        fireworks->setupParticleProperties(overlayLayer, Vec2(m_visibleSize.width * 0.5f, 0), "fworks.png");
    });
    auto sequenceBg               = Sequence::create(DelayTime::create(1.0f), fadeInBg, showItsChildren, nullptr);
    bg->runAction(sequenceBg);

    // swallow touches
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);  // Initialize with default value
    listener->onTouchBegan = AX_CALLBACK_2(VictoryGameLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void VictoryGameLayer::setGameSceneReference(GameScene* gameScene)
{
    m_gameScene = gameScene;
}

bool VictoryGameLayer::onTouchBegan(ax::Touch* touch, ax::Event* event)
{
    // If the game has started, let the touches propagate through
    return true;
}

void VictoryGameLayer::displayCurrentLevel()
{
    auto stars      = "Level 1";
    auto levelLabel = Label::createWithBMFont(fonts::hugmat_fnt, stars);
    levelLabel->setBMFontSize(50.0f);
    levelLabel->setColor(Color3B::BLACK);
    levelLabel->setPositionX(bg->getContentSize().width / 2.0f);
    levelLabel->setPositionY(bg->getContentSize().height * 0.8f);
    bg->addChild(levelLabel);
}
