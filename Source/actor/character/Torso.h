#pragma once

#include "DynamicEntity.h"

namespace torso_NS
{
inline const float m_hX = 15.0f;
inline const float m_hY = 25.0f;
}  // namespace torso_NS

class Torso : public DynamicEntity
{
public:
    Torso();
    virtual ~Torso() = default;
    bool showOnGameScene(const ax::Vec2& position) override;  // staticEntity
    void update(float dt) override;                           // dynamicEntity
    inline float getHY() const { return torso_NS::m_hY; };
    inline float getHX() const { return torso_NS::m_hX; };

private:
    float m_scale;
    float m_angle;
    void defineBody(float x, float y);  // create physic body
};
