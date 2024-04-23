#include "Ball.h"

#include "GameUtils.h"
#include "Constants.h"
#include "helpers/Logger.h"
#include "scene/GameScene.h"
#include "scene/SceneManager.h"
#include "helpers/box2d/b2BodyBuilder.h"
#include "particlez/SmokeyParticle.h"
#include "Audio.h"

using namespace category_bits;
using namespace physics;

namespace
{
// for collision filtering
constexpr auto kBallCategoryBits = kBall;
constexpr auto kBallMaskBits     = kHead | kHair | kWall | kWallRight | kWallLeft | kItem;
constexpr auto m_radius          = kBallRadius;
}  // namespace

//  // 8
Ball::Ball() : desiredBounceHeight{6.0f}, m_isHitHair{}, m_isHitWall{}
// m_radius{ 20.0f }
{
    GameUtils::PPM::initVars();
    m_scene = SceneManager::getInstance().getCurrentScene<GameScene>();
}

bool Ball::showOnGameScene(const Vec2& position)
{
    if (m_isShownOnGameScene)
    {
        return false;
    }

    m_isShownOnGameScene = true;

    // create the sprite
    m_bodySprite = Sprite::create("circle.png");

    m_bodySprite->setColor(Color3B::GREEN);

    // set initial sprite position
    m_bodySprite->setPosition(position);

    defineBody(position.x, position.y);

    // scale it down
    float scale = m_radius / (m_bodySprite->getContentSize().width / 2.0f);
    m_bodySprite->setScale(scale);

    // add to this scene
    auto snmg = static_cast<GameScene*>(SceneManager::getInstance().getCurrentScene());
    snmg->addChild(m_bodySprite, graphical_layers::kBall);

    // add smoke to it with trail
    // auto smoke = SmokeyParticle::create();
    // smoke->setupParticleProperties(snmg, position, "smoke_01.png");

    // streak
    streak = MotionStreak::create(0.2f, 3.0f, 5.0f, Color3B::WHITE, "light_01.png");
    snmg->addChild(streak);

    return true;
}

void Ball::update(float dt)
{
    if (!m_body)
    {
        return;
    }

    DynamicEntity::update(dt);

    if (m_scene->_isStartGame)
        desiredBounceHeight = 10.0f;

    b2Vec2 gravityForce = b2Vec2(0.0f, -m_body->GetMass() * 10.0f);  // Set the desired force
    m_body->ApplyForceToCenter(gravityForce, false);

    if (getIsHitHair() && !m_scene->_isReviveGame)
    {
        if (m_scene->_isStartGame && (!m_scene->_isWinGame || !m_scene->_isGameOver))
        {
            m_scene->m_hud->updateCurrentMissionValues();
        }

        // play bounce sound
        if (m_scene->_isStartGame)
        {
            Audio::getInstance().playSfx(std::string(sound_audio::head_ball));
        }

        // Calculate the correction needed to reach the desired bounce height.
        float currentHeight = m_body->GetPosition().y;

        // new
        float timeToDesiredHeight = sqrt(2.0f * (desiredBounceHeight - currentHeight) / -physics::kGravity);
        float velocityY           = -physics::kGravity * timeToDesiredHeight;

        // Apply the correction to the ball's velocity.
        b2Vec2 newVelocity = m_body->GetLinearVelocity();
        newVelocity.y      = velocityY;
        m_body->SetLinearVelocity(newVelocity);

        // Reset collision flag after processing bounce
        this->setIsHitHair(false);
    }

    if (streak)
    {
        streak->setPosition(m_bodySprite->getPosition());
    }
}

void Ball::setGameOver(bool isGameOver)
{
    if (!m_scene->_isGameOver && !m_scene->_isWinGame)
    {
        m_scene->_isGameOver = isGameOver;
    }
}

void Ball::defineBody(float x, float y)
{
    auto snMgr = SceneManager::getInstance().getCurrentScene<GameScene>();

    b2BodyBuilder bodyBuilder(snMgr->getWorld());
    m_body = bodyBuilder
                 .type(b2_dynamicBody)
                 //.awake(false)
                 .position(x, y, GameUtils::PPM::kPpm)
                 .buildBody();

    bodyBuilder.newCircleFixture(m_radius, GameUtils::PPM::kPpm)
        .denstity(0.8f)     // 0.25
        .friction(1.0f)     // 0.2
        .restitution(0.0f)  // 0.5
        .categoryBits(kBallCategoryBits)
        .maskBits(kBallMaskBits)
        .setUserData(this)
        .buildFixture();
}
