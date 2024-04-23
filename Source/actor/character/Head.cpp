#include "Head.h"

#include "GameUtils.h"
#include "Constants.h"
#include "helpers/Logger.h"
#include "scene/GameScene.h"
#include "scene/SceneManager.h"
#include "helpers/box2d/b2BodyBuilder.h"
#include "actor/character/Torso.h"

using namespace category_bits;

namespace
{
	// head
	constexpr float kHeadRadius = 28.0f;
	constexpr auto kHeadCategoryBits = kHead;
	constexpr auto kHeadMaskBits = kWall | kBall | kWallLeft | kWallRight;

	// hair
	//constexpr auto kHairCategoryBits = kHair;
	//constexpr auto kHairMaskBits = kWall | kBall | kWallLeft | kWallRight | kMeteor;
	//constexpr auto kHairMaskBits = kBall;
}

Head::Head(DynamicEntity* parent) :
	m_parent{ parent },
	m_radius{},
	m_scale{ 1.0f }
{
	GameUtils::PPM::initVars();
}

bool Head::showOnGameScene(const Vec2& position)
{
	if (m_isShownOnGameScene)
	{
		return false;
	}

	m_isShownOnGameScene = true;

	// create the sprite
	m_bodySprite = Sprite::create("head.png");

	// scale it down
	m_bodySprite->setScale(m_scale);
	//blinkToRed();

	defineBody(position.x, position.y);

	// set initial sprite position because it depends on another bodies position
	m_bodySprite->setPosition({ m_body->GetPosition().x * GameUtils::PPM::kPpm, m_body->GetPosition().y * GameUtils::PPM::kPpm });

	// add to this scene
	auto snmg = static_cast<GameScene*>(SceneManager::getInstance().getCurrentScene());
	snmg->addChild(m_bodySprite);

	return true;
}

void Head::update(float dt)
{
	if (!m_body)
	{
		return;
	}

	DynamicEntity::update(dt);
}

void Head::blinkToRed()
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

void Head::defineBody(float x, float y)
{
	auto snMgr = SceneManager::getInstance().getCurrentScene<GameScene>();

	// m_radius = 20.0f;
	m_radius = kHeadRadius;

	// new
	auto otherBody = dynamic_cast<Torso*>(m_parent);
	auto& positionOtherBody = otherBody->getBody()->GetPosition();
	float angleOtherBody = otherBody->getBody()->GetAngle();
	b2Vec2 relativeOffset(0, otherBody->getHY() / GameUtils::PPM::kPpm);
	auto positionThisBody = positionOtherBody + b2Mul(b2Rot(angleOtherBody), relativeOffset);
	positionThisBody = positionThisBody - b2Mul(b2Rot(angleOtherBody), b2Vec2(0, -m_radius / GameUtils::PPM::kPpm));

	b2BodyBuilder bodyBuilder(snMgr->getWorld());
	m_body = bodyBuilder.type(b2_dynamicBody)
		.position(positionThisBody.x, positionThisBody.y, 1)
		.buildBody();

	bodyBuilder.newCircleFixture(m_radius, m_scale, GameUtils::PPM::kPpm)
		.denstity(1.0f)
		.friction(0.5f) // 0.2
		.restitution(0.5f)
		.categoryBits(kHeadCategoryBits)
		.maskBits(kHeadMaskBits)
		.setUserData(this)
		.buildFixture();
}