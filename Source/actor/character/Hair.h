#pragma once
#include "DynamicEntity.h"

class Hair : public DynamicEntity
{
public:
    explicit Hair(DynamicEntity* parent);
    ~Hair() override = default;
    bool showOnGameScene(const ax::Vec2& position) override;  // staticEntity
    void update(float dt) override;                           // dynamicEntity
    inline float getRadius() const { return m_radius; }
    inline void setOnHitHair(bool onHitHair) { _isOnHitHair = onHitHair; }

private:
    DynamicEntity* m_parent;
    bool _isOnHitHair{false};
    float m_scale;
    float m_radius;
    float m_angle;
    void defineBody(float x, float y);  // create physic body
};
