#include "Wall.h"

#include "scene/GameScene.h"
#include "scene/SceneManager.h"
#include "helpers/box2d/b2BodyBuilder.h"
#include "GameUtils.h"
#include "Constants.h"
#include "MyEnums.h"

using namespace ax;
using namespace category_bits;

namespace
{
constexpr auto kWallCategoryBits = kWall;
constexpr auto kWallMaskBits     = kTorso | kBall | kItem;
}  // namespace

Wall::Wall()
{
    GameUtils::PPM::initVars();
}

bool Wall::showOnGameScene(const Vec2& position)
{
    if (m_isShownOnGameScene)
    {
        return false;
    }

    m_isShownOnGameScene = true;

    // create the sprite
    // m_bodySprite = Sprite::create("shade.png");

    // set initial sprite position
    // m_bodySprite->setPosition(position);

    // create the physics body
    // use its content size to calculate radius
    defineBody(position.x, position.y);

    return true;
}

void Wall::update(float dt) {}

void Wall::defineBody(float x, float y)
{
    auto snMgr = SceneManager::getInstance().getCurrentScene<GameScene>();

    b2BodyBuilder bodyBuilder(snMgr->getWorld());

    m_body = bodyBuilder.type(b2_staticBody).position(x, y, GameUtils::PPM::kPpm).buildBody();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    float xPos       = visibleSize.width;   // 312
    float yPos       = visibleSize.height;  //

    // top wall 0.01f
    bodyBuilder
        .newRectangleFixture(xPos / 2, yPos * 0.1f, b2Vec2(xPos / 2, yPos), 0,
                             GameUtils::PPM::kPpm)  // ground
        .categoryBits(kWallCategoryBits)
        .maskBits(0)
        .setUserData(reinterpret_cast<void*>(WallFixtureType::CEILING))
        .buildFixture();

    auto top        = Sprite::create("topbar.png");
    float topScaleX = (xPos) / top->getContentSize().width;
    float topScaleY = (yPos * 0.1f) / top->getContentSize().height;
    top->setScale(topScaleX, topScaleY);
    // top->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    // btmRight->setPosition((m_body->GetPosition().x + (xPos - 10) / GameUtils::PPM::kPpm) * GameUtils::PPM::kPpm,
    // (m_body->GetPosition().y + (yPos * 0.36f) / GameUtils::PPM::kPpm) * GameUtils::PPM::kPpm);
    top->setPosition(xPos / 2, (m_body->GetPosition().y + (yPos) / GameUtils::PPM::kPpm) * GameUtils::PPM::kPpm);
    snMgr->addChild(top);

    // bottom wall
    // the height of the box is half the screen height (top on the screen and bottom off the screen)
    bodyBuilder
        .newRectangleFixture(xPos / 2, yPos * 0.2f, b2Vec2(xPos / 2, 0), 0,
                             GameUtils::PPM::kPpm)  // ground
        .restitution(0.2f)
        .denstity(1.0f)
        .categoryBits(kWallCategoryBits)
        .maskBits(kWallMaskBits)
        .setUserData(reinterpret_cast<void*>(WallFixtureType::GROUND))
        .buildFixture();

    // roadTexture_27
    auto btm        = Sprite::create("ground.png");
    float btmScaleX = (xPos) / btm->getContentSize().width;
    float btmScaleY = (yPos * 0.2f) / btm->getContentSize().height;
    btm->setScale(btmScaleX, btmScaleY);
    btm->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    btm->setPosition(0, 0);
    snMgr->addChild(btm, graphical_layers::kGround);

    // grassfg on ground
    auto btmGrass        = Sprite::create("grassfg.png");
    float btmGrassScaleX = (xPos) / btmGrass->getContentSize().width;
    float btmGrassScaleY = (yPos * 0.2f) / btmGrass->getContentSize().height;
    btmGrass->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    btmGrass->setPosition(btm->getContentSize().width / 2, btm->getContentSize().height - 5);
    btm->addChild(btmGrass, 1);

    // left wall
    bodyBuilder
        .newRectangleFixture(1, visibleSize.height / 2, b2Vec2(0, yPos / 2), 0,
                             GameUtils::PPM::kPpm)  // left wall
        .restitution(0.5f)
        .friction(1.0f)
        .categoryBits(category_bits::kWallLeft)
        .maskBits(kWallMaskBits)
        .setUserData(reinterpret_cast<void*>(WallFixtureType::LEFT_WALL))
        .buildFixture();

    // right wall
    bodyBuilder
        .newRectangleFixture(1, visibleSize.height / 2, b2Vec2(xPos - 0, yPos / 2), 0,
                             GameUtils::PPM::kPpm)  // right wall
        .restitution(0.5f)
        .friction(1.0f)
        .categoryBits(category_bits::kWallRight)
        .maskBits(kWallMaskBits)
        .setUserData(reinterpret_cast<void*>(WallFixtureType::RIGHT_WALL))
        .buildFixture();

    {
        // Create circle fixtures in a straight line on the ground
        float circleRadius = 0.05f;                                                             // Set radius of circles
        int numCircles     = (visibleSize.height / GameUtils::PPM::kPpm) / (2 * circleRadius);  // Number of circles

        for (int i = 0; i < numCircles; ++i)
        {
            float yxPos = (i * 2.0f * circleRadius) + circleRadius;  // Distance between circles

            b2CircleShape circleShape;
            circleShape.m_radius = circleRadius;
            // circleShape.m_p.Set((visibleSize.width * 0.2f) / GameUtils::PPM::kPpm, yxPos);
            circleShape.m_p.Set(0 / GameUtils::PPM::kPpm, yxPos);

            b2FixtureDef circleFixtureDef;
            circleFixtureDef.shape               = &circleShape;
            circleFixtureDef.density             = 1.0f;  // Set density as needed
            circleFixtureDef.restitution         = 0.5f;
            circleFixtureDef.friction            = 1.0f;
            circleFixtureDef.filter.categoryBits = category_bits::kWallLeft;
            circleFixtureDef.filter.maskBits     = kWallMaskBits;
            circleFixtureDef.userData.pointer =
                reinterpret_cast<uintptr_t>(reinterpret_cast<void*>(WallFixtureType::LEFT_WALL));
            m_body->CreateFixture(&circleFixtureDef);

            auto left       = Sprite::create("lollipopFruitYellow.png");
            float leftScale = circleRadius * GameUtils::PPM::kPpm / left->getContentSize().width;
            left->setScale(leftScale * 2.0f);
            left->setPosition(m_body->GetPosition().x * GameUtils::PPM::kPpm, yxPos * GameUtils::PPM::kPpm);
            // left->setPosition(m_body->GetPosition().x * GameUtils::PPM::kPpm, (m_body->GetPosition().y + (yPos *
            // 0.37f) / GameUtils::PPM::kPpm) * GameUtils::PPM::kPpm);
            snMgr->addChild(left, -1);
        }
    }

    // ------------right----------
    {
        // Create circle fixtures in a straight line on the ground
        float circleRadius = 0.05f;                                                             // Set radius of circles
        int numCircles     = (visibleSize.height / GameUtils::PPM::kPpm) / (2 * circleRadius);  // Number of circles

        for (int i = 0; i < numCircles; ++i)
        {
            float yxPos = (i * 2.0f * circleRadius) + circleRadius;  // Distance between circles

            b2CircleShape circleShape;
            circleShape.m_radius = circleRadius;
            // circleShape.m_p.Set((visibleSize.width * 0.2f) / GameUtils::PPM::kPpm, yxPos);
            circleShape.m_p.Set((visibleSize.width) / GameUtils::PPM::kPpm, yxPos);

            b2FixtureDef circleFixtureDef;
            circleFixtureDef.shape               = &circleShape;
            circleFixtureDef.density             = 1.0f;  // Set density as needed
            circleFixtureDef.restitution         = 0.5f;
            circleFixtureDef.friction            = 1.0f;
            circleFixtureDef.filter.categoryBits = category_bits::kWallLeft;
            circleFixtureDef.filter.maskBits     = kWallMaskBits;
            circleFixtureDef.userData.pointer =
                reinterpret_cast<uintptr_t>(reinterpret_cast<void*>(WallFixtureType::LEFT_WALL));
            m_body->CreateFixture(&circleFixtureDef);

            auto right       = Sprite::create("lollipopFruitYellow.png");
            float rightScale = circleRadius * GameUtils::PPM::kPpm / right->getContentSize().width;
            right->setScale(rightScale * 2.0f);
            right->setPosition(
                m_body->GetPosition().x + ((visibleSize.width) / GameUtils::PPM::kPpm) * GameUtils::PPM::kPpm,
                yxPos * GameUtils::PPM::kPpm);
            snMgr->addChild(right, -1);
        }
    }

    bodyBuilder
        .newRectangleFixture(25.0f, 25.0f, b2Vec2(xPos - 10, yPos * 0.21f), 0.8f,
                             GameUtils::PPM::kPpm)  // right wall
        .categoryBits(kWallCategoryBits)
        .maskBits(kWallMaskBits)
        .setUserData(reinterpret_cast<void*>(WallFixtureType::BOTTOM_RIGHT))
        .buildFixture();

    // bottom right rotated wall
    // Assuming you have a method to convert Box2D positions to Cocos2d-x positions
    auto convertToWorldSpace = [](const b2Vec2& position) {
        return Vec2(position.x * GameUtils::PPM::kPpm, position.y * GameUtils::PPM::kPpm);
    };

    auto btmRight = Sprite::create("tanks_crateWood.png");
    btmRight->setPosition((m_body->GetPosition().x + (xPos - 10) / GameUtils::PPM::kPpm) * GameUtils::PPM::kPpm,
                          (m_body->GetPosition().y + (yPos * 0.21f) / GameUtils::PPM::kPpm) * GameUtils::PPM::kPpm);
    btmRight->setRotation(AX_RADIANS_TO_DEGREES(0.8f));
    snMgr->addChild(btmRight, graphical_layers::kRightBox);

    // circle bottom left
    bodyBuilder
        .newCircleFixture(b2Vec2(0, yPos * 0.2f), 50, GameUtils::PPM::kPpm)
        //.denstity(0.8f) // 0.25
        //.friction(1.0f) // 0.2
        //.restitution(0.0f)//0.5
        .categoryBits(category_bits::kWallBushLeft)
        .maskBits(category_bits::kTorso)
        .setUserData(reinterpret_cast<void*>(WallFixtureType::BOTTOM_LEFT))
        .buildFixture();

    _bushSprite        = Sprite::create("bush.png");
    float btmLeftScale = 50 / _bushSprite->getContentSize().width;
    _bushSprite->setScale(btmLeftScale * 2.5f);
    _bushSprite->setPosition(m_body->GetPosition().x * GameUtils::PPM::kPpm,
                             (m_body->GetPosition().y + (yPos * 0.2f) / GameUtils::PPM::kPpm) * GameUtils::PPM::kPpm);
    snMgr->addChild(_bushSprite, graphical_layers::kLeftBush);
}
