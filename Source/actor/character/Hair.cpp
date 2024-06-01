#include "Hair.h"

#include "GameUtils.h"
#include "Constants.h"
#include "helpers/Logger.h"
#include "scene/GameScene.h"
#include "scene/SceneManager.h"
#include "helpers/box2d/b2BodyBuilder.h"
#include "actor/character/Head.h"

using namespace category_bits;

USING_NS_AX;

namespace
{
// Hair
constexpr float kHairRadius      = 40.0f;  // 28
constexpr auto kHairCategoryBits = kHair;
constexpr auto kHairMaskBits     = kWall | kBall | kWallLeft | kWallRight;
}  // namespace

Hair::Hair(DynamicEntity* parent) : m_parent{parent}, m_radius{}, m_scale{1.0f}
{
    GameUtils::PPM::initVars();
}

bool Hair::showOnGameScene(const Vec2& position)
{
    if (m_isShownOnGameScene)
    {
        return false;
    }

    m_isShownOnGameScene = true;

    // create the sprite
    m_bodySprite = Sprite::create("hair.png");

    // scale it down
    m_bodySprite->setScale(m_scale);

    defineBody(position.x, position.y);

    // set initial sprite position because it depends on another bodies position
    m_bodySprite->setPosition(
        {m_body->GetPosition().x * GameUtils::PPM::kPpm, m_body->GetPosition().y * GameUtils::PPM::kPpm});

    // add to game scene
    auto snmg = static_cast<GameScene*>(SceneManager::getInstance().getCurrentScene());
    snmg->addChild(m_bodySprite);

    return true;
}

void Hair::update(float dt)
{
    if (!m_body)
    {
        return;
    }

    DynamicEntity::update(dt);
}

void Hair::defineBody(float x, float y)
{
    auto snMgr = SceneManager::getInstance().getCurrentScene<GameScene>();

    // m_radius = 20.0f;
    m_radius = kHairRadius / GameUtils::PPM::kPpm;

    // new
    auto otherBody          = dynamic_cast<Head*>(m_parent);
    auto& positionOtherBody = otherBody->getBody()->GetPosition();
    float angleOtherBody    = otherBody->getBody()->GetAngle();
    b2Vec2 relativeOffset(0, otherBody->getRadius() * 0.8f / GameUtils::PPM::kPpm);
    auto positionThisBody = positionOtherBody + b2Mul(b2Rot(angleOtherBody), relativeOffset);

    // new body
    // Create a body definition
    b2BodyDef boxBodyDef;
    boxBodyDef.type = b2_dynamicBody;
    boxBodyDef.position.Set(positionThisBody.x, positionThisBody.y);
    m_body = snMgr->getWorld()->CreateBody(&boxBodyDef);

    const float hairRadius = (m_radius);  // defines the hair segment's radius
    const int numSegments  = 8;           // number of segments for the semi-circle
    float yOffset          = hairRadius * 0.8f;

    std::vector<b2Vec2> hairVertices;
    for (int i = numSegments / 4; i <= (numSegments * 3) / 4; ++i)
    {
        float angle = b2_pi * i / static_cast<float>(numSegments);
        float xh    = hairRadius * std::cos(angle);
        float yh    = hairRadius * std::sin(angle) - yOffset;
        hairVertices.push_back(b2Vec2(xh, yh));
    }

    // Create a b2PolygonShape object
    b2PolygonShape hairShape;

    // Populate the shape with the circular segment vertices
    b2Vec2* segmentVertices = hairVertices.data();
    int numVertices         = hairVertices.size();
    hairShape.Set(segmentVertices, numVertices);

    // Create a fixture definition
    b2FixtureDef hairFixtureDef;
    hairFixtureDef.shape               = &hairShape;
    hairFixtureDef.density             = 1.0f;
    hairFixtureDef.friction            = 0.1f;
    hairFixtureDef.filter.categoryBits = kHairCategoryBits;
    hairFixtureDef.filter.maskBits     = kHairMaskBits;

    // Attach the hair fixture to the circle body
    m_body->CreateFixture(&hairFixtureDef);
}
