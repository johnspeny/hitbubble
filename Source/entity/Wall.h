#pragma once

#include "DynamicEntity.h"

class Wall : public DynamicEntity
{
private:
	void defineBody(float x, float y);
public:
	Wall();
	~Wall() = default;
	bool showOnGameScene(const Vec2& position) override;
	void update(float dt) override;

	ax::Sprite* _bushSprite;
};
