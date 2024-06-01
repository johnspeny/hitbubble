#pragma once

#include "StaticEntity.h"
#include "box2d/box2d.h"

enum HumanSide
{
    LeftSide,
    RightSide
};

class DynamicEntity : public StaticEntity
{
public:
    DynamicEntity();
    virtual ~DynamicEntity();
    virtual void update(float dt);
    virtual bool showOnGameScene(const Vec2& position) override = 0;
    virtual void setPosition(const Vec2& position) override;
    virtual void setPosition(const Vec2& position, float angle);

    inline b2Body* getBody() const { return m_body; }
    virtual void destroyBody();
    virtual bool removeFromGameScene() override;

protected:
    b2Body* m_body;
};
