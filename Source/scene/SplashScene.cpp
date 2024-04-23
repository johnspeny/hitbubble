#include "SplashScene.h"
#include "AudioEngine.h"
#include "Constants.h"
#include "GameScene.h"
#include "scene/SceneManager.h"
#include "ui/UILoadingBar.h"

using namespace ax;
using namespace ui;

SplashScene::SplashScene() :
	numOfResLoaded{ 0 },
	logoSprite{}
{
	m_origin = Director::getInstance()->getVisibleOrigin();
	m_visibleSize = Director::getInstance()->getVisibleSize();
}

bool SplashScene::init()
{
	if (!ax::Scene::init()) return false;

	// Set the background color to white
	auto background = LayerColor::create(Color4B::WHITE);
	addChild(background, INT_MIN);  // Ensure the background is at the back

	// display logo here
	displayLogo();

	// text
	displayLogoText();

	// load resources into cache in the loop
	// 3 seconds timer
	scheduleOnce(AX_SCHEDULE_SELECTOR(SplashScene::startLoop), 3.0f);

	return true;
}

void SplashScene::loadResources()
{
	// load audio file
	std::array<std::string_view, 7> audioFiles = {
		sound_audio::btn_click,
		sound_audio::coin_collect,
		sound_audio::head_ball,
		sound_audio::sad_crowd,
		sound_audio::applause,
		sound_audio::thud,
		sound_audio::screams
	};

	for (const auto& audioFile : audioFiles)
	{
		ax::AudioEngine::preload(audioFile);
	}
	numOfResLoaded++;
}

void SplashScene::updateResources(float dt)
{
	switch (numOfResLoaded)
	{
	case 0: loadResources(); break;
	case 1: {
		auto scene = utils::createInstance<GameScene>();
		SceneManager::getInstance().replaceScene(scene);
		scene->startNewGame();
	}; break;
	default:
		break;
	}
}

void SplashScene::loadSpriteFrames(std::string_view file)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(file);
}

void SplashScene::displayLogo()
{
	// create a logo
	logoSprite = ax::Sprite::create(art::logo);
	addChild(logoSprite);

	// Calculate the scaling factor
	float scaleX = m_visibleSize.width / logoSprite->getContentSize().width;
	float scaleY = m_visibleSize.height / logoSprite->getContentSize().height;

	// Choose the smaller scaling factor to ensure the logo fits entirely within the screen
	float scale = std::min(scaleX, scaleY);

	float additionalScale{ 0.25f }; // Adjust this value as per your requirements

	// scale
	logoSprite->setScale(scale * additionalScale);

	// position the sprite on the center of the screen
	logoSprite->setPosition(
		ax::Vec2(
			static_cast<float>(m_visibleSize.width) / 2.0f + m_origin.x,
			static_cast<float>(m_visibleSize.height) / 2.0f + m_origin.y
		)
	);
}

void SplashScene::displayLogoText()
{
	float paddingY = 20.0f;
	// LabelTTF
	TTFConfig ttfConfig("fonts/SupplyCenter-0W9nz.ttf", 24.0f);
	auto label2 = Label::createWithTTF(ttfConfig, "HOLEYGONS", TextHAlignment::CENTER, m_visibleSize.width);
	label2->setColor(Color3B::BLACK);
	addChild(label2, 0);
	float yPos = logoSprite->getPositionY() - logoSprite->getContentSize().height / 2 * logoSprite->getScale();
	label2->setPosition(Vec2(m_visibleSize.width / 2, yPos - paddingY));

	// animate it
	// Set the initial properties of the logo text
	label2->setScale(0.5f);  // Set the initial scale to a smaller size
	label2->setOpacity(0);   // Set the initial opacity to fully transparent

	// Create the actions for the animation
	auto fadeIn = FadeIn::create(1.0f);           // Fade-in action with a duration of 1 second
	auto scaleUp = ScaleTo::create(1.0f, 1.0f);   // Scale-up action to reach a scale of 1.0 over 1 second

	// Create a sequence of actions to perform them in order
	auto sequence = Sequence::create(fadeIn, scaleUp, nullptr);

	// Run the sequence on the logo text
	label2->runAction(sequence);
}

void SplashScene::startLoop(float dt)
{
	schedule(AX_SCHEDULE_SELECTOR(SplashScene::updateResources));
}