#pragma once

#include "DynamicEntity.h"

USING_NS_AX;

class Head : public DynamicEntity
{
public:
	explicit Head(DynamicEntity* parent);
	~Head() override = default;
	bool showOnGameScene(const Vec2& position) override; // staticEntity
	void update(float dt) override; // staticEntity
	inline float getRadius() const { return m_radius; }
	inline void setOnHitHair(bool onHitHair) { _isOnHitHair = onHitHair; }

	void blinkToRed();

private:
	DynamicEntity* m_parent;
	bool _isOnHitHair{ false };
	float m_scale;
	float m_radius;
	float m_angle;
	void defineBody(float x, float y); // create physic body
};