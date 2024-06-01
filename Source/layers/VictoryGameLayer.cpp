#include "VictoryGameLayer.h"
#include "Constants.h"
#include "scene/GameScene.h"
#include "MainMenuLayer.h"
#include "Audio.h"
#include "particlez/FireWorksParticle.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"

#include "helpers/string_manipulation/zlibString.h"
#include "helpers/string_manipulation/base64.h"
#include "GameUtils.h"

USING_NS_AX;
using namespace ui;

VictoryGameLayer::VictoryGameLayer(int seasonId, int levelId, int coin)
    : m_seasonIndex{seasonId}, m_levelIndex{levelId}, m_collectedCoins{coin}
{
    m_origin      = Director::getInstance()->getVisibleOrigin();
    m_visibleSize = Director::getInstance()->getVisibleSize();
}

VictoryGameLayer* VictoryGameLayer::create(int seasonId, int levelId, int coin)
{
    auto pRet = new VictoryGameLayer(seasonId, levelId, coin);
    if (pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        delete pRet;
        pRet = nullptr;
    }
    return pRet;
}

bool VictoryGameLayer::init()
{
    if (!Layer::init())
        return false;

    // save coins collected
    UserDefault::getInstance()->setIntegerForKey("Coin", m_collectedCoins);
    UserDefault::getInstance()->flush();

    goToNextLevel();

    // play bounce sound
    Audio::getInstance().playSfx(std::string(sound_audio::applause));

    auto bgLayer = LayerColor::create(Color4B(0, 0, 0, 200));
    this->addChild(bgLayer);

    // overlay
    overlayLayer = Layer::create();
    this->addChild(overlayLayer);

    // bg
    bg = ax::ui::ImageView::create("victory.png", Widget::TextureResType::LOCAL);
    bg->setPosition(Vec2(m_visibleSize.width / 2.0f, m_visibleSize.height * 0.5f));
    bg->setScale(m_visibleSize.width * 0.75f / bg->getContentSize().width,
                 m_visibleSize.height / bg->getContentSize().height / 2);
    bg->setScale9Enabled(true);
    bg->setOpacity(0);
    overlayLayer->addChild(bg, -1);

    // Create a button
    auto retryButton = ui::Button::create("btn_continue.png", "", "");
    retryButton->setScale9Enabled(true);
    retryButton->setPosition(Vec2(bg->getContentSize().width / 2.0f, -retryButton->getContentSize().height));
    bg->addChild(retryButton);

    retryButton->setOpacity(0);
    auto fadeIn  = FadeIn::create(0.5f);
    auto scaleUp = ScaleTo::create(0.5f, 1.0f);

    ax::CallFunc* pressButton = ax::CallFunc::create([this, retryButton]() {
        // Add a click event listener to the button
        retryButton->addClickEventListener([=, this](Ref* sender) {
            if (this->isVisible())
            {
                this->setVisible(false);  // Hide the layer when the button is clicked
                // start playing the game
                if (m_gameScene)
                {
                    // reset game
                    m_gameScene->resetGame();
                }
            }
        });
    });

    auto sequence =
        Sequence::create(DelayTime::create(1.5f), Spawn::create(fadeIn, scaleUp, nullptr), pressButton, nullptr);
    retryButton->runAction(sequence);

    // actions of bg
    auto fadeInBg                 = FadeIn::create(0.5f);
    ax::CallFunc* showItsChildren = ax::CallFunc::create([this]() {
        // level
        displayCurrentLevel();
        auto fireworks = FireWorksParticle::create();
        fireworks->setupParticleProperties(overlayLayer, Vec2(m_visibleSize.width * 0.5f, 0), "fworks.png");
    });
    auto sequenceBg               = Sequence::create(DelayTime::create(1.0f), fadeInBg, showItsChildren, nullptr);
    bg->runAction(sequenceBg);

    // swallow touches
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);  // Initialize with default value
    listener->onTouchBegan = AX_CALLBACK_2(VictoryGameLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void VictoryGameLayer::setGameSceneReference(GameScene* gameScene)
{
    m_gameScene = gameScene;
}

bool VictoryGameLayer::onTouchBegan(ax::Touch* touch, ax::Event* event)
{
    // If the game has started, let the touches propagate through
    return true;
}

void VictoryGameLayer::displayCurrentLevel()
{
    auto level      = StringUtils::format("Level %s", std::to_string(m_levelIndex).c_str());
    auto levelLabel = Label::createWithBMFont(fonts::supplycenter_fnt, level);
    levelLabel->setBMFontSize(50.0f);
    levelLabel->setColor(Color3B::BLACK);
    levelLabel->setPositionX(bg->getContentSize().width / 2.0f);
    levelLabel->setPositionY(bg->getContentSize().height * 0.8f);
    bg->addChild(levelLabel);
}

void VictoryGameLayer::goToNextLevel()
{
    // ---------------From Asset / new ------------------
    // load data from json file
    auto newJsonData = FileUtils::getInstance()->getDataFromFile(game_data::season_level_filepath);
    std::string newcontentString((const char*)newJsonData.getBytes(), newJsonData.getSize());

    // parse json
    rapidjson::Document new_document;
    new_document.Parse<0>(newcontentString.c_str());

    // Check if parsing of the new_document JSON succeeded
    if (!new_document.IsObject())
    {
        AXLOG("Failed to parse new_document JSON data.");
        return;
    }

    // -----------------------Existing ----------------
    // get writable path
    std::string path = FileUtils::getInstance()->getWritablePath() + "season_level.json";

    // load data from json file
    auto jsonData = FileUtils::getInstance()->getDataFromFile(path);
    std::string contentString((const char*)jsonData.getBytes(), jsonData.getSize());

    // decrypt/decode the data
    auto decode               = Strings::from_base64(contentString);
    auto decodedContentString = zlibString::decompress_string(decode);

    // parse json
    rapidjson::Document writableDoc;
    writableDoc.Parse<0>(decodedContentString.c_str());

    // Check if parsing of the writable JSON succeeded
    if (!writableDoc.IsObject())
    {
        AXLOG("Failed to parse writable JSON data.");
        return;
    }

    // ------------------ merge data----------------------
    // Merge the data from updatedDoc into writableDoc
    // Check if the array key exists in both the updated JSON and writable JSON
    if (new_document.HasMember("seasons") && writableDoc.HasMember("seasons"))
    {
        // Access the arrays
        rapidjson::Value& updatedArray  = new_document["seasons"];
        rapidjson::Value& writableArray = writableDoc["seasons"];

        // Find season object in json
        for (size_t i{0}; i < writableArray.Size(); i++)
        {
            auto& season = writableArray[i];
            if (season.HasMember("id") && season["id"].IsInt() && season["id"].GetInt() == m_seasonIndex)
            {
                // Find the level within the season
                auto& levelsArray = season["levels"];
                for (size_t j = 0; j < levelsArray.Size(); j++)
                {
                    auto& level = levelsArray[j];
                    if (level.HasMember("id") && level["id"].IsInt() && level["id"].GetInt() == m_levelIndex)
                    {
                        if (level.HasMember("id") && level["id"].IsInt())
                        {
                            level["completed"] = true;

                            // Check if the next level should be unlocked
                            int nextLevelIndex = j + 1;
                            if (nextLevelIndex < levelsArray.Size())
                            {
                                auto& nextLevel = levelsArray[nextLevelIndex];
                                if (nextLevel.HasMember("id") && nextLevel["id"].IsInt() &&
                                    nextLevel["id"].GetInt() == nextLevelIndex + 1)
                                {
                                    nextLevel["unlocked"] = true;

                                    UserDefault::getInstance()->setIntegerForKey("CurrentSeasonIndex", i);
                                    UserDefault::getInstance()->flush();

                                    UserDefault::getInstance()->setIntegerForKey("CurrentLevelIndex", nextLevelIndex);
                                    UserDefault::getInstance()->flush();
                                }
                            }
                        }
                        break;
                    }
                }
                break;  // no need to continue searching the loop when id has been found hence exit it with a break
            }
        }
    }

    // save
    GameUtils::saveItemDataToJson(writableDoc, "season_level.json");
}
