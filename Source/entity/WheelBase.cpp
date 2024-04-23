#include "WheelBase.h"
#include "scene/GameScene.h"
#include "util/box2d/b2BodyBuilder.h"
#include "GameUtils.h"
#include "Constants.h"

#define BOARD_BODY_CATEGORY_BITS kBoard

using namespace category_bits;

WheelBase::WheelBase(GameScene* gameScene) :
	m_gameScene{ gameScene }
{
	GameUtils::PPM::initVars();
}

WheelBase::~WheelBase()= default;

bool WheelBase::showOnGameScene(const Vec2& position)
{
	if (m_isShownOnGameScene)
	{
		return false;
	}

	m_isShownOnGameScene = true;

	// create the sprite
	m_bodySprite = Sprite::create("shade.png");

	// set initial sprite position
	m_bodySprite->setPosition(position);

	// create the physics body
	// use its content size to calculate radius
	defineBody(position.x, position.y);

	// add to this scene
	m_gameScene->addChild(m_bodySprite);

	return true;
}

void WheelBase::defineBody(float x, float y)
{
	b2BodyBuilder bodyBuilder(m_gameScene->getWorld());

	m_body = bodyBuilder.type(b2_staticBody)
		.position(x, y, GameUtils::PPM::kPpm)
		.buildBody();

	bodyBuilder.newRectangleFixture(1.5f, 1.5f, GameUtils::PPM::kPpm)
		.categoryBits(kBoard)
		.maskBits(0)
		//.groupIndex(-1)
		.buildFixture();
}