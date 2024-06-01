#include "Torso.h"

#include "GameUtils.h"
#include "Constants.h"
#include "helpers/Logger.h"
#include "scene/GameScene.h"
#include "scene/SceneManager.h"
#include "helpers/box2d/b2BodyBuilder.h"

using namespace category_bits;
using namespace torso_NS;

namespace
{
constexpr auto kTorsoCategoryBits = kTorso;
constexpr auto kTorsoMaskBits     = kWall | kHead | kWallLeft | kWallRight | kBall | kWallBushLeft;
}  // namespace

Torso::Torso()
{
    GameUtils::PPM::initVars();
}

bool Torso::showOnGameScene(const Vec2& position)
{
    if (m_isShownOnGameScene)
    {
        return false;
    }

    m_isShownOnGameScene = true;

    // create the sprite
    m_bodySprite = Sprite::create("body.png");

    // set initial sprite position
    m_bodySprite->setPosition(position);

    defineBody(position.x, position.y);

    // scale it down
    float scaleX = m_hX / (m_bodySprite->getContentSize().width / 2.0f);
    float scaleY = m_hY / (m_bodySprite->getContentSize().height / 2.0f);
    m_bodySprite->setScale(scaleX, scaleY);

    // add to this scene
    auto snmg = static_cast<GameScene*>(SceneManager::getInstance().getCurrentScene());
    snmg->addChild(m_bodySprite);

    return true;
}

void Torso::update(float dt)
{
    if (!m_body)
    {
        return;
    }

    DynamicEntity::update(dt);
}

void Torso::defineBody(float x, float y)
{
    // shape dimension
    // m_hX = 10.0f;
    // m_hY = 30.0f;

    // old
    auto snMgr = SceneManager::getInstance().getCurrentScene<GameScene>();

    b2BodyBuilder bodyBuilder(snMgr->getWorld());
    m_body = bodyBuilder.type(b2_dynamicBody)
                 .position(x, y, GameUtils::PPM::kPpm)
                 .rotation(true)
                 //.angle(0.0f)
                 .buildBody();

    bodyBuilder.newRectangleFixture(m_hX, m_hY, GameUtils::PPM::kPpm)
        .denstity(20.0f)
        .friction(0.2f)  // 0.2f
        .categoryBits(kTorsoCategoryBits)
        .maskBits(kTorsoMaskBits)
        .setUserData(this)
        .buildFixture();
}
