#include "MainMenuLayer.h"
#include "Constants.h"
#include "scene/GameScene.h"
#include "helpers/Logger.h"
#include "helpers/string_manipulation/zlibString.h"
#include "helpers/string_manipulation/base64.h"

USING_NS_AX;
using namespace ui;

MainMenuLayer::MainMenuLayer() : m_touchBgImage{}
{
    m_origin      = Director::getInstance()->getVisibleOrigin();
    m_visibleSize = Director::getInstance()->getVisibleSize();

    // get total stored coin
    m_totalCoin = UserDefault::getInstance()->getIntegerForKey("Coin", 0);

    // current season screen
    m_currentSeasonIndex = UserDefault::getInstance()->getIntegerForKey("CurrentSeasonIndex", 0);
    m_currentLevelIndex  = UserDefault::getInstance()->getIntegerForKey("CurrentLevelIndex", 0);
}

MainMenuLayer* MainMenuLayer::create()
{
    auto pRet = new MainMenuLayer();
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

bool MainMenuLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(255, 0, 0, 0)))
        return false;

    CLOG("MainMenu");
    loadSeasonFromJsonFile();
    createSeason();

    // auto l = m_seasons[m_currentSeasonIndex]->getName();
    auto& l = m_seasons[m_currentSeasonIndex]->getLevels().at(0);

    // Create a button -------------------------------------------------------------------------
    auto playButton = ui::Button::create("transparentDark40.png", "", "");
    playButton->setPosition(Vec2(m_visibleSize.width / 2.0f, m_visibleSize.height * 0.5f));
    playButton->setScale9Enabled(true);
    playButton->setSwallowTouches(true);
    addChild(playButton);

    auto swipe_hand = Sprite::create("swipehand.png");
    swipe_hand->setScale(0.5f);
    swipe_hand->setPosition(Vec2(m_visibleSize.width / 2.0f, m_visibleSize.height * 0.16f));
    addChild(swipe_hand);

    auto swipe_tut = Sprite::create("swipe_tut.png");
    swipe_tut->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    swipe_tut->setPosition(Vec2(swipe_hand->getContentSize().width, 0));
    swipe_hand->addChild(swipe_tut);

    // actions
    float duration = 2.0f;  // Adjust the duration as needed

    auto moveToRight = EaseInOut::create(
        MoveTo::create(duration, Vec2(m_visibleSize.width * 0.8f, m_visibleSize.height * 0.16f)), 2.0f);
    auto moveToLeft = EaseInOut::create(
        MoveTo::create(duration, Vec2(m_visibleSize.width * 0.2f, m_visibleSize.height * 0.16f)), 2.0f);

    auto sequence      = Sequence::create(moveToRight, moveToLeft, nullptr);
    auto repeatForever = RepeatForever::create(sequence);

    swipe_hand->runAction(repeatForever);

    // Add a click event listener to the button
    playButton->addClickEventListener([=](Ref* sender) {
        if (this->isVisible())
        {
            this->setVisible(false);  // Hide the layer when the button is clicked
            // start playing the game
            if (m_gameScene)
            {
                m_gameScene->playGame();
                m_gameScene->_isStartGame = true;
                updateSwallowTouches();

                // Remove the MainMenuLayer from its parent
                this->removeFromParentAndCleanup(true);
            }
        }
    });

    // swallow touches
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(m_swallowTouches);  // Initialize with default value
    listener->onTouchBegan = AX_CALLBACK_2(MainMenuLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    m_touchListener = listener;  // Store the listener

    return true;
}

void MainMenuLayer::updateSwallowTouches()
{
    // Set m_swallowTouches based on the game state
    m_swallowTouches = !m_gameScene || !m_gameScene->_isStartGame;

    // Set the state for the listener
    if (m_touchListener)
    {
        m_touchListener->setSwallowTouches(m_swallowTouches);
    }
}

void MainMenuLayer::setGameSceneReference(GameScene* gameScene)
{
    m_gameScene = gameScene;
    // get current level
    auto items = m_seasons[m_currentSeasonIndex]->getLevels()[m_currentLevelIndex]->items;
    m_gameScene->setCurrentIndices(m_currentSeasonIndex + 1, m_currentLevelIndex + 1, items);
    updateSwallowTouches();
}

bool MainMenuLayer::onTouchBegan(ax::Touch* touch, ax::Event* event)
{
    if (!m_gameScene->_isStartGame)
    {
        // If the game hasn't started yet, swallow the touches
        return true;
    }

    return false;
}

void MainMenuLayer::loadSeasonFromJsonFile()
{
    // load data from json file
    auto contentSrc = FileUtils::getInstance()->getDataFromFile(game_data::season_level_filepath);
    // write data to external storage
    auto fileUtils   = FileUtils::getInstance();
    std::string path = fileUtils->getWritablePath() + "season_level.json";
    if (!fileUtils->isFileExist(path))
    {
        // then read from writable path
        auto content = fileUtils->getDataFromFile(game_data::season_level_filepath);
        std::string contentString((const char*)content.getBytes(), content.getSize());

        // compress the data
        auto enc  = zlibString::compress_string(contentString);
        auto enc2 = Strings::to_base64(enc);

        fileUtils->writeStringToFile(enc2, path);
    }
}

void MainMenuLayer::createSeason()
{
    // get writable path
    std::string path = FileUtils::getInstance()->getWritablePath() + "season_level.json";

    // load data from json file
    auto jsonData = FileUtils::getInstance()->getDataFromFile(path);
    std::string contentString((const char*)jsonData.getBytes(), jsonData.getSize());

    // decrypt/decode the data
    auto decode               = Strings::from_base64(contentString);
    auto decodedContentString = zlibString::decompress_string(decode);

    // parse json
    rapidjson::Document m_document;
    m_document.Parse<0>(decodedContentString.c_str());

    // populate season and level with data from json document
    if (!m_document.HasParseError())
    {
        // get the season array from the json document
        auto& seasonsArray = m_document["seasons"];

        // iterate through every season in the array
        m_totalSeasons = static_cast<int>(seasonsArray.Size());

        for (rapidjson::SizeType i = 0; i < m_totalSeasons; i++)
        {
            auto& seasonObject = seasonsArray[i];

            // get the 'name' property of the season
            std::string seasonName = seasonObject["name"].GetString();

            // create a Season Object
            auto season = std::make_unique<Season>(seasonName);

            // add the season object to this a pool to select level
            m_seasons.push_back(std::move(season));

            // Get the 'levels' array for the current season
            rapidjson::Value& levelsArray = seasonObject["levels"];

            // Iterate over each level in the array
            for (rapidjson::SizeType j = 0; j < levelsArray.Size(); j++)
            {
                rapidjson::Value& levelObject = levelsArray[j];

                // Get the properties of the level
                int levelID           = levelObject["id"].GetInt();
                std::string levelName = levelObject["name"].GetString();
                bool isCompleted      = levelObject["completed"].GetBool();
                int items             = levelObject["items"].GetInt();
                bool isUnlocked       = levelObject["unlocked"].GetBool();

                // Create a Level object and Add the level to the season
                auto level           = std::make_unique<Level>();
                level->id            = levelID;
                level->name          = levelName;
                level->completed     = isCompleted;
                level->unlocked      = isUnlocked;
                level->items         = items;
                level->pointsAwarded = false;

                // Create a Level object and Add the level to the season
                m_seasons.back()->addLevel(std::move(level));
            }
        }
    }
    else
    {
        // ax::log("json has errors");
        return;
    }
    // m_seasons[m_currentSeasonIndex]->getName()
}
