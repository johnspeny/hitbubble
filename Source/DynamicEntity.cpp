#include "DynamicEntity.h"
#include "GameUtils.h"

DynamicEntity::DynamicEntity() :
	m_body{}
{
}

DynamicEntity::~DynamicEntity()
{
}

void DynamicEntity::update(float dt)
{
	if (!m_body)
	{
		return;
	}

	b2Vec2 b2bodyPos = m_body->GetPosition();
	m_bodySprite->setPosition(b2bodyPos.x * GameUtils::PPM::kPpm, b2bodyPos.y * GameUtils::PPM::kPpm);

	// rotation to match that of the body
	float bodyAngle = m_body->GetAngle();

	// Convert the angle from radians to degrees
	float degrees = AX_RADIANS_TO_DEGREES(-bodyAngle);

	// Set the rotation of the sprite
	m_bodySprite->setRotation(degrees);
}

void DynamicEntity::setPosition(const Vec2& position)
{
	m_body->SetTransform({ position.x / GameUtils::PPM::kPpm, position.y / GameUtils::PPM::kPpm }, m_body->GetAngle());
}

void DynamicEntity::setPosition(const Vec2& position, float angle)
{
	m_body->SetTransform({ position.x / GameUtils::PPM::kPpm, position.y / GameUtils::PPM::kPpm }, angle);
}

void DynamicEntity::destroyBody()
{
	if (!m_body)
	{
		return;
	}
	m_body->GetWorld()->DestroyBody(m_body);
	m_body = nullptr;
}

bool DynamicEntity::removeFromGameScene()
{
	if (StaticEntity::removeFromGameScene() == false)
	{
		return false;
	}
	destroyBody();
	return true;
}