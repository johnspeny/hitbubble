#include "Fruit.h"
#include "scene/GameScene.h"
#include "scene/SceneManager.h"
#include "helpers/box2d/b2BodyBuilder.h"
#include "GameUtils.h"
#include "helpers/Logger.h"
#include "Constants.h"

using namespace category_bits;

namespace
{
// for collision filtering
constexpr auto kFruitCategoryBits = kItem;
constexpr auto kFruitMaskBits     = kBall | kWall | kWallRight | kWallLeft;
}  // namespace

Fruit::Fruit(int lifeVal) : m_radius{30.0f}, m_lifeVal{lifeVal}
{
    m_scene = SceneManager::getInstance().getCurrentScene<GameScene>();
}

bool Fruit::showOnGameScene(const Vec2& position)
{
    if (m_isShownOnGameScene)
        return false;

    m_isShownOnGameScene = true;

    auto& spriteName = selectRandomSpriteName(this->getSpriteNames());

    // create the sprite
    m_bodySprite = Sprite::create("circle.png");

    // set initial sprite position
    m_bodySprite->setPosition(position);

    // set names
    // m_bodySprite->setName(spriteName);

    // scale it down
    float scale = m_radius / (m_bodySprite->getContentSize().width / 2.0f);
    m_bodySprite->setScale(scale);

    // m_bodySprite->setTag();

    // create the physics body
    // use its content size to calculate radius
    defineBody(position.x, position.y);

    // add to this scene
    auto snmg = SceneManager::getInstance().getCurrentScene<GameScene>();
    snmg->addChild(m_bodySprite, 1000);

    // add the label to the fruit
    lifeLabel = Label::createWithBMFont(fonts::hugmat_fnt, std::to_string(m_lifeVal));
    lifeLabel->setBMFontSize(18.0f);
    lifeLabel->setPosition({m_bodySprite->getContentSize().width * 0.5f, m_bodySprite->getContentSize().height * 0.5f});
    lifeLabel->setColor(Color3B::BLUE);
    m_bodySprite->addChild(lifeLabel);

    return true;
}

void Fruit::update(float dt)
{
    Item::update(dt);
    if (m_isHitWall && !_isHitWallHandled)
    {
        m_isHitWall       = false;
        _isHitWallHandled = true;

        float newRadius = 15.0f;

        b2Fixture* fixtureToDelete = m_body->GetFixtureList();
        m_body->DestroyFixture(fixtureToDelete);
        // Update Box2D body's fixture size
        b2CircleShape newShape;
        newShape.m_radius = newRadius / GameUtils::PPM::kPpm;

        // Create a new fixture with the updated shape
        b2FixtureDef fixtureDef;
        fixtureDef.shape               = &newShape;
        fixtureDef.density             = 0.1f;
        fixtureDef.friction            = 0.0f;
        fixtureDef.restitution         = 0.2f;
        fixtureDef.filter.categoryBits = kItem;
        fixtureDef.filter.maskBits     = kWall | kWallRight | kWallLeft;
        fixtureDef.userData.pointer    = reinterpret_cast<uintptr_t>(this);

        m_body->CreateFixture(&fixtureDef);

        // Update sprite's size
        float scale = newRadius / (m_bodySprite->getContentSize().width / 2.0f);
        m_bodySprite->setScale(scale);

        //// Destroy the existing fixture
        // b2Fixture* fixtureToDelete = itemFixture->GetBody()->GetFixtureList();
        // itemFixture->GetBody()->DestroyFixture(fixtureToDelete);

        //// Update Box2D body's fixture size
        // b2CircleShape newShape;
        // newShape.m_radius = item->getRadius() / GameUtils::PPM::kPpm;

        //// Create a new fixture with the updated shape
        // b2FixtureDef fixtureDef;
        // fixtureDef.shape               = &newShape;
        // fixtureDef.density             = 0.1f;
        // fixtureDef.friction            = 0.0f;
        // fixtureDef.restitution         = 0.2f;
        // fixtureDef.filter.categoryBits = kItem;
        // fixtureDef.filter.maskBits     = kWall | kWallRight | kWallLeft;

        // itemFixture->GetBody()->CreateFixture(&fixtureDef);

        //// Update sprite's size
        // float scale = item->getRadius() / (item->getBodySprite()->getContentSize().width / 2.0f);
        // item->getBodySprite()->setScale(scale);
    }
}

const std::string& Fruit::selectRandomSpriteName(const std::vector<std::string>& spriteNames) const
{
    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Randomly select a sprite name
    std::uniform_int_distribution<> dist(0, static_cast<int>(spriteNames.size()) - 1);
    int randomIndex                     = dist(gen);
    const std::string& randomSpriteName = spriteNames[randomIndex];
    return randomSpriteName;
}

void Fruit::startContact()
{
    Item::startContact();

    // update the label to reduce the life value
    m_lifeVal--;
    lifeLabel->setString(std::to_string(m_lifeVal));
}

void Fruit::setIsHitWall(bool ishitWall)
{
    m_isHitWall = ishitWall;
}

void Fruit::defineBody(float x, float y)
{
    auto snMgr = SceneManager::getInstance().getCurrentScene<GameScene>();

    b2BodyBuilder bodyBuilder(snMgr->getWorld());
    m_body = bodyBuilder.type(b2_dynamicBody).position(x, y, GameUtils::PPM::kPpm).buildBody();

    bodyBuilder.newCircleFixture(m_radius, GameUtils::PPM::kPpm)
        .denstity(0.1f)
        .friction(0.0)
        .restitution(0.2f)
        .categoryBits(kFruitCategoryBits)
        .maskBits(kFruitMaskBits)
        .setUserData(this)
        .buildFixture();
}
