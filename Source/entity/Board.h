#pragma once

#include "DynamicEntity.h"

class Board : public DynamicEntity
{
public:
	Board();
	~Board() = default;
	virtual bool showOnGameScene(const Vec2& position) override;
	virtual void update(float dt) override;
	inline float getRadius() const { return m_radius; }

private:
	float m_radius;
	void defineBody(float x, float y);
	int generateRandomAngular(int min, int max);
	const std::string& selectRandomSpriteName(const std::vector<std::string>& spriteNames) const;
	inline std::vector<std::string> getSpriteNames() const {
		return { "will.png", "greenplate.png", "greyplate.png" };
	}
};
