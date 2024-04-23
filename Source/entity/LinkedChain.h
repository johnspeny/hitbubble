#pragma once

#include "DynamicEntity.h"

class LinkedChain : public DynamicEntity
{
public:
	LinkedChain();
	virtual ~LinkedChain() = default;

	bool showOnGameScene(const Vec2& position) override;
	const std::vector<b2Body*>& getChainEnds() const;
    void update(float dt) override;

private:
	void defineBody(float x, float y);
	std::vector<b2Body*> m_chainEnds; // store top and bottom of the chain
    std::unordered_map<b2Body*, ax::Sprite*> bodySpriteMap;
};
