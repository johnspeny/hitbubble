#pragma once

#include "DynamicEntity.h"

USING_NS_AX;

class Hair : public DynamicEntity
{
public:
	explicit Hair(DynamicEntity* parent);
	~Hair() override = default;
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
	void getFixtureDefsFromPhysicsEditor(const std::string& name, b2Body* new_body, float scaleFactor);
    b2Body* createCircularSegment(b2World* world, float radius, float startAngle, float endAngle, b2Vec2 position, float ppm);
};