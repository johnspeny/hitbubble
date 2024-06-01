#include "LinkedChain.h"
#include "scene/GameScene.h"
#include "scene/SceneManager.h"
#include "helpers/box2d/b2BodyBuilder.h"
#include "GameUtils.h"
#include "Constants.h"

using namespace category_bits;
USING_NS_AX;

namespace
{
constexpr auto kChainCategoryBits = kChain;
constexpr auto kChainMaskBits     = kWall;
}  // namespace

LinkedChain::LinkedChain()
{
    GameUtils::PPM::initVars();
}

bool LinkedChain::showOnGameScene(const Vec2& position)
{
    if (m_isShownOnGameScene)
    {
        return false;
    }

    m_isShownOnGameScene = true;

    defineBody(position.x, position.y);

    return true;
}

const std::vector<b2Body*>& LinkedChain::getChainEnds() const
{
    return m_chainEnds;
}

void LinkedChain::defineBody(float x, float y)
{
    auto snMgr = SceneManager::getInstance().getCurrentScene<GameScene>();

    b2BodyBuilder bodyBuilder(snMgr->getWorld());

    b2RevoluteJointDef revJointDef;
    b2Body* link;
    float chain_hX = 5.0f;   // width
    float chain_hY = 10.0f;  // height

    // first chain in the link
    link = bodyBuilder.type(b2_dynamicBody).position(x, y, GameUtils::PPM::kPpm).angle(0.0f).buildBody();

    m_chainEnds.push_back(link);

    bodyBuilder.newRectangleFixture(chain_hX, chain_hY, GameUtils::PPM::kPpm)
        .denstity(2.0f)
        .friction(0.2f)
        .angularDamping(0.2f)
        .linearDamping(0.1f)
        .restitution(0.5f)
        .categoryBits(kChainCategoryBits)
        .maskBits(kChainMaskBits)
        .setUserData(this)
        .buildFixture();

    auto spriteF = Sprite::create("box.png");
    // float leftScale = circleRadius * GameUtils::PPM::kPpm / left->getContentSize().width;
    // left->setScale(leftScale * 2.0f);
    spriteF->setPosition(link->GetPosition().x * GameUtils::PPM::kPpm, link->GetPosition().y * GameUtils::PPM::kPpm);
    snMgr->addChild(spriteF);

    bodySpriteMap[link] = spriteF;

    int numberOfChains{8};

    for (int i = 0; i < numberOfChains; i++)
    {
        // obtain position of previous body
        auto& otherBodyPosition = link->GetPosition();
        float otherBodyAngle    = link->GetAngle();
        b2Vec2 relativeOffset   = {0, -chain_hY * 0.75f / GameUtils::PPM::kPpm};

        // next body settings
        auto nextBodyPosition = otherBodyPosition + b2Mul(b2Rot(otherBodyAngle), relativeOffset);
        nextBodyPosition =
            nextBodyPosition - b2Mul(b2Rot(otherBodyAngle), b2Vec2(0, chain_hY * 0.75f / GameUtils::PPM::kPpm));

        // apply the settings to next chain
        b2Body* nextLink = bodyBuilder.type(b2_dynamicBody)
                               .position(nextBodyPosition.x, nextBodyPosition.y, 1)
                               .angle(0.0f)
                               .buildBody();

        bodyBuilder.newRectangleFixture(chain_hX, chain_hY, GameUtils::PPM::kPpm)
            .denstity(2.0f)
            .friction(0.2f)
            //.angularDamping(0.2f)
            //.linearDamping(0.1f)
            .restitution(0.5f)
            .categoryBits(kChainCategoryBits)
            .maskBits(kChainMaskBits)
            .setUserData(this)
            .buildFixture();

        auto sprite = Sprite::create("box.png");
        // float leftScale = circleRadius * GameUtils::PPM::kPpm / left->getContentSize().width;
        // left->setScale(leftScale * 2.0f);
        sprite->setPosition(nextLink->GetPosition().x * GameUtils::PPM::kPpm,
                            nextLink->GetPosition().y * GameUtils::PPM::kPpm);
        snMgr->addChild(sprite);

        // join previous chain to next chain to form a link
        revJointDef.bodyA        = link;
        revJointDef.bodyB        = nextLink;
        revJointDef.localAnchorA = {0, -chain_hY * 0.75f / GameUtils::PPM::kPpm};
        revJointDef.localAnchorB = {0, chain_hY * 0.75f / GameUtils::PPM::kPpm};
        snMgr->getWorld()->CreateJoint(&revJointDef);

        // next link to be previous link
        link = nextLink;

        // m_chainMiddles.push_back(nextLink);
        m_chainEnds.push_back(link);
        bodySpriteMap[link] = sprite;
    }
}

void LinkedChain::update(float dt)
{
    for (const auto& pair : bodySpriteMap)
    {
        auto body   = pair.first;
        auto sprite = pair.second;

        b2Vec2 b2bodyPos = body->GetPosition();
        sprite->setPosition(b2bodyPos.x * GameUtils::PPM::kPpm, b2bodyPos.y * GameUtils::PPM::kPpm);

        // rotation to match that of the body
        float bodyAngle = body->GetAngle();

        // Convert the angle from radians to degrees
        float degrees = AX_RADIANS_TO_DEGREES(-bodyAngle);

        // Set the rotation of the sprite
        sprite->setRotation(degrees);
    }
}
