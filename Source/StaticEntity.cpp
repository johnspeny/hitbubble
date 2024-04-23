#include "StaticEntity.h"
#include "scene/SceneManager.h"

StaticEntity::StaticEntity() :
	m_bodySprite{},
	m_isShownOnGameScene{ false }
{
}

StaticEntity::~StaticEntity()
{
}

bool StaticEntity::showOnGameScene(const Vec2& position)
{
	if (m_isShownOnGameScene)
	{
		return false;
	}

	m_isShownOnGameScene = true;

	// set intial position of entity
	m_bodySprite->setPosition(position);

	// add to current running scene
	auto currentScene = SceneManager::getInstance().getCurrentScene();

	// add to this scene
	currentScene->addChild(m_bodySprite);

	return true;
}

bool StaticEntity::removeFromGameScene()
{
	if (m_isShownOnGameScene == false)
	{
		return false;
	}

	m_isShownOnGameScene = false;

	// add to current running scene
	auto currentScene = SceneManager::getInstance().getCurrentScene();

	// remove sprite from scene
	currentScene->removeChild(m_bodySprite);

	return true;
}

void StaticEntity::setPosition(const Vec2& position)
{
	// set intial position of entity
	m_bodySprite->setPosition(position);
}