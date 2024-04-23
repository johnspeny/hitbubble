#pragma once

#include "DynamicEntity.h"

USING_NS_AX;

class GameScene;

class Ball : public DynamicEntity
{
public:
	Ball();
	virtual ~Ball() override = default;
	bool showOnGameScene(const Vec2& position) override; // staticEntity
	void update(float dt) override; // staticEntity
	//inline float getRadius() const { return m_radius; }

	// hit hair
	inline void setIsHitHair(bool ishitHair) { m_isHitHair = ishitHair; }
	inline bool getIsHitHair() const { return m_isHitHair; }

	// hit wall
	inline void setIsHitWall(bool ishitWall) { m_isHitWall = ishitWall; }
	inline bool getIsHitWall() const { return m_isHitWall; }

	void setGameOver(bool isGameOver);

private:
	float desiredBounceHeight;
	bool m_isHitHair;
	bool m_isHitWall;
	GameScene* m_scene;
	ax::MotionStreak* streak;

	float m_scale;
	//float m_radius;
	float m_angle;
	void defineBody(float x, float y); // create physic body

	int count{ 0 };
};