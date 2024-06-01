#pragma once
#include "box2d/box2d.h"

// Define the collision event ID
constexpr auto COLLISION_EVENT_ID = "collision_event";

class WorldContactListener : public b2ContactListener
{
public:
    WorldContactListener();
    virtual ~WorldContactListener() = default;

    virtual void BeginContact(b2Contact* contact) override;
    virtual void EndContact(b2Contact* contact) override;
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

private:
    b2Fixture* GetTargetFixture(short targetCategoryBits, b2Fixture* f1, b2Fixture* f2) const;
};
