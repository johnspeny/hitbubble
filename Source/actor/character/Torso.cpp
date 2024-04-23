#include "Torso.h"

#include "GameUtils.h"
#include "Constants.h"
#include "helpers/Logger.h"
#include "scene/GameScene.h"
#include "scene/SceneManager.h"
#include "helpers/box2d/b2BodyBuilder.h"

using namespace category_bits;
using namespace torso_NS;

namespace
{
	constexpr auto kTorsoCategoryBits = kTorso;
	constexpr auto kTorsoMaskBits = kWall | kHead | kWallLeft | kWallRight | kBall | kWallBushLeft;
}

Torso::Torso()
{
	GameUtils::PPM::initVars();
}

bool Torso::showOnGameScene(const Vec2& position)
{
	if (m_isShownOnGameScene)
	{
		return false;
	}

	m_isShownOnGameScene = true;

	// create the sprite
	m_bodySprite = Sprite::create("body.png");

	// set initial sprite position
	m_bodySprite->setPosition(position);

	// scale it down
	// m_bodySprite->setScale(m_scale);

	defineBody(position.x, position.y);

	// scale it down
	float scaleX = m_hX / (m_bodySprite->getContentSize().width / 2.0f);
	float scaleY = m_hY / (m_bodySprite->getContentSize().height / 2.0f);
	m_bodySprite->setScale(scaleX, scaleY);

	// add to this scene
	auto snmg = static_cast<GameScene*>(SceneManager::getInstance().getCurrentScene());
	snmg->addChild(m_bodySprite);

	return true;
}

void Torso::update(float dt)
{
	if (!m_body)
	{
		return;
	}

	DynamicEntity::update(dt);
}

void Torso::blinkToRed()
{
	/**************************************************************************/

// Save the original color of the sprite
	Color3B originalColor = m_bodySprite->getColor();

	// Create an action to change the color to red
	ActionInterval* turnRed = TintTo::create(0.2f, 255, 0, 0);

	// Create an action to revert the color to the original color
	ActionInterval* turnBack = TintTo::create(0.2f, originalColor.r, originalColor.g, originalColor.b);

	// Create a sequence of actions to blink
	Sequence* blinkSequence = Sequence::create(turnRed, turnBack, nullptr);

	// Repeat the blink sequence forever
	//RepeatForever* blinkForever = RepeatForever::create(blinkSequence);

	//m_bodySprite->runAction(blinkForever);

	// Repeat the blink sequence for 3 seconds
	Repeat* blinkRepeat = Repeat::create(blinkSequence, 3);

	// Run the blink action
	m_bodySprite->runAction(blinkRepeat);

	// Delay for 3 seconds before stopping the blinking
	DelayTime* delay = DelayTime::create(3.0f);

	// Create a FiniteTimeAction to stop the blinking
	FiniteTimeAction* stopBlinking = CallFunc::create([this]() {
		m_bodySprite->stopAllActions();
		});

	/**************************************************************************/
}

void Torso::defineBody(float x, float y)
{
	// shape dimension
	//m_hX = 10.0f;
	//m_hY = 30.0f;

	// old
	auto snMgr = SceneManager::getInstance().getCurrentScene<GameScene>();

	b2BodyBuilder bodyBuilder(snMgr->getWorld());
	m_body = bodyBuilder.type(b2_dynamicBody)
		.position(x, y, GameUtils::PPM::kPpm)
		.rotation(true)
		//.angle(0.0f)
		.buildBody();

	bodyBuilder.newRectangleFixture(m_hX, m_hY, GameUtils::PPM::kPpm)
		.denstity(20.0f)
		.friction(0.2f) // 0.2f
		.categoryBits(kTorsoCategoryBits)
		.maskBits(kTorsoMaskBits)
		.setUserData(this)
		.buildFixture();

	// leg // no sprites for you
	//m_body->
//	bodyBuilder.newRectangleFixture(30, m_hY * 0.25f, b2Vec2(0, 0 - m_hY + (m_hY * 0.25f)), 0, GameUtils::PPM::kPpm) // right wall
//		.categoryBits(kTorsoCategoryBits)
//		.maskBits(kTorsoMaskBits)
//		.setUserData(this)
//		.buildFixture();
}