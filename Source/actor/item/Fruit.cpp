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
