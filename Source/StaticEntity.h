#ifndef __STATIC_ENTITY_H__
#define __STATIC_ENTITY_H__

#include "axmol.h"

using namespace ax;

class StaticEntity
{
public:
	StaticEntity();
	virtual ~StaticEntity();
	virtual bool showOnGameScene(const Vec2& position);
	virtual bool removeFromGameScene();
	virtual void setPosition(const Vec2& position);
	inline Sprite* getBodySprite() const { return m_bodySprite; }

protected:
	Sprite* m_bodySprite;
	bool m_isShownOnGameScene;
};

#endif // !__STATIC_ENTITY_H__
