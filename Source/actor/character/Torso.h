#pragma once

#include "DynamicEntity.h"

USING_NS_AX;

namespace torso_NS
{
	inline const float m_hX = 15.0f;
	inline const float m_hY = 25.0f;
}

class Torso : public DynamicEntity
{
public:
	Torso();
	virtual ~Torso() = default;
	bool showOnGameScene(const Vec2& position) override; // staticEntity
	void update(float dt) override; // staticEntity
	inline float getHY() const { return torso_NS::m_hY; };
	inline float getHX() const { return torso_NS::m_hX; };

	void blinkToRed();

private:
	float m_scale;
	float m_angle;
	void defineBody(float x, float y); // create physic body
};