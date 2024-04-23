#ifndef __WHEELBASE_H__
#define __WHEELBASE_H__

#include "DynamicEntity.h"

class GameScene;

class WheelBase : public DynamicEntity
{
private:
	GameScene* m_gameScene;
	void defineBody(float x, float y);
public:
	WheelBase(GameScene* gameScene);
	~WheelBase();
	virtual bool showOnGameScene(const Vec2& position) override;
	//virtual void update(float dt) override;
};

#endif // !__WHEELBASE_H__