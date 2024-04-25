#include "GameScene.h"
#include "GameUtils.h"
#include "Constants.h"
#include "helpers/Logger.h"
#include "MyEnums.h"
#include "particlez/DustParticle.h"
#include "Audio.h"
#include "actor/item/Fruit.h"

GameScene::GameScene()
    : _isUpdatePaused{false}
    , _isStartGame{false}
    , _isGameOver{false}
    , _isWinGame{false}
    , _isReviveGame{false}
    , timeScale{1.0f}
    , m_b2dr{}
{
    m_visibleSize = Director::getInstance()->getVisibleSize();
    m_origin      = Director::getInstance()->getVisibleOrigin();
    GameUtils::PPM::initVars();
}

GameScene::~GameScene()
{
    // Destroy any joints connected to bodies
    b2Joint* joint = m_world->GetJointList();
    while (joint)
    {
        b2Joint* nextJoint = joint->GetNext();
        m_world->DestroyJoint(joint);
        joint = nextJoint;
    }

    // Destroy bodies
    b2Body* body = m_world->GetBodyList();
    while (body)
    {
        b2Body* nextBody = body->GetNext();
        m_world->DestroyBody(body);
        body = nextBody;
    }

    // clean up contact listener
    m_contactListener = nullptr;

    // clean up world
    m_world = nullptr;
}

GameScene* GameScene::create()
{
    auto pRet = new GameScene();
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

bool GameScene::init()
{
    if (!ax::Scene::init())
        return false;

    m_hud = Hud::create();
    m_hud->setGameSceneReference(this);
    addChild(m_hud, 99);

    // create mainmenu layer
    m_mainMenu = MainMenuLayer::create();
    m_mainMenu->setGameSceneReference(this);
    addChild(m_mainMenu, 9999);

    // create bg
    auto bgSprite = Sprite::create("bg.png");
    bgSprite->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);
    auto bgScaleX = m_visibleSize.width / bgSprite->getContentSize().width;
    auto bgScaleY = m_visibleSize.height / bgSprite->getContentSize().height;
    bgSprite->setScale(bgScaleX, bgScaleY);
    this->addChild(bgSprite);

    // create world
    createWorld(b2Vec2(0, physics::kGravity));

    // Create b2DebugRenderer
    renderDrawDebugger(m_world.get(), true);

    // Handle player control
    auto touchListener          = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = AX_CALLBACK_2(GameScene::onTouchBegan, this);
    touchListener->onTouchMoved = AX_CALLBACK_2(GameScene::onTouchMoved, this);
    touchListener->onTouchEnded = AX_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    // tick the box2d world
    schedule(AX_SCHEDULE_SELECTOR(GameScene::update));

    return true;
}

void GameScene::update(float dt)
{
    switch (_gameState)
    {
    case GameState::init:
    {
        _gameState = GameState::update;
        break;
    }

    case GameState::update:
    {
        if (!_isUpdatePaused)
        {
            updatePhysicsWorld(dt);
            updatePlayer(dt);
            updateBall(dt);

            if (_isStartGame)
            {  // update board
                m_board->update(dt);
                m_chain->update(dt);

                // bush shake
                onShakeBush();

                // collect coins
                onCollectCoin();

                // update items
                updateItems(dt);

                onItemsAllDetached();
            }

            // Check for game over conditions
            if (_isGameOver && !_isGameOverHandled)
            {
                _gameState         = GameState::gameover;
                _isGameOverHandled = true;
            }

            if (_isWinGame && !_isWinGameHandled)
            {
                _gameState        = GameState::victory;
                _isWinGameHandled = true;
            }

            if (_isReviveGame && !_isReviveGameHandled)
            {
                _gameState           = GameState::revive;
                _isReviveGameHandled = true;
            }
        }
        break;
    }

    case GameState::pause:
    {
        /////////////////////////////
        // Add your codes below...like....
        //
        // anyPauseStuff()
        _isUpdatePaused = true;
        unschedule(AX_SCHEDULE_SELECTOR(GameScene::update));

        break;
    }

    case GameState::end:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // CleanUpMyCrap();
        break;
    }

    case GameState::revive:
    {
        this->unschedule("checkCoinRemoved");

        onReviveGame();
        _isGameOver = false;
        _gameState  = GameState::update;
        break;
    }

    case GameState::gameover:
    {
        this->unschedule("che*ckCoinRemoved");

        onGameOver();
        _isGameOver = false;
        _gameState  = GameState::update;
        break;
    }

    case GameState::victory:
    {
        this->unschedule("checkCoinRemoved");

        onGameWin();
        _isWinGame = false;
        _gameState = GameState::update;
        break;
    }
    }
}

void GameScene::updatePhysicsWorld(float dt)
{
    // step box2d world
    m_world->Step(fixedTimeStep * timeScale, physics::kVelocityIterations, physics::kPositionIterations);

    // delete items just after steps
    removeMeteor(dt);
}

void GameScene::updatePlayer(float dt)
{
    // update character parts
    if (m_character)
    {
        m_character->update(dt);
        if (m_isTouched && !m_isTouchedOnCannon)
        {
            Vec2 currentPosition = m_character->m_torso->getBodySprite()->getPosition();
            Vec2 direction       = targetTouchPos - currentPosition;

            float speed = 800.0f;  // Adjust the speed factor as needed

            // Only consider the x-component for movement
            auto movement = Vec2(direction.x, 0);

            // Limit the cannon's movement within the screen
            movement.x = clampf(movement.x, -speed * fixedTimeStep, speed * fixedTimeStep);

            // Set the velocity of the cannon's physics body
            m_character->m_torso->getBody()->SetLinearVelocity(
                b2Vec2(movement.x / GameUtils::PPM::kPpm / fixedTimeStep, 0));

            const float positionTolerance = 1.0f;

            // Check if the cannon has reached the target position
            if (std::abs(currentPosition.x - targetTouchPos.x) < positionTolerance)
            {
                m_isTouchedOnCannon = true;
                m_isTouched         = false;  // Stop further updates
            }
        }
    }
}

void GameScene::updateBall(float dt)
{
    if (m_ball)
    {
        m_ball->update(dt);
    }
}

bool GameScene::onTouchBegan(ax::Touch* touch, ax::Event* event)
{
    initialTouchPos = touch->getLocation();
    m_isTouched     = true;
    targetTouchPos  = initialTouchPos;

    // Check if the touch is on the cannon sprite
    if (isTouchingItem(touch))
    {
        m_isTouchedOnCannon = true;
        return true;
    }
    return true;
}

void GameScene::onTouchMoved(ax::Touch* touch, ax::Event* event)
{
    // Only proceed if the touch is on the cannon
    if (m_isTouchedOnCannon)
    {
        // Get the touch position and limit it within the desired range
        Vec2 touchPos = touch->getLocation();
        float minX    = _director->getVisibleSize().width * 0.2f;
        float maxX    = _director->getVisibleSize().width * 0.8f;

        if (touchPos.x >= minX && touchPos.x <= maxX)
        {
            // Set the target touch position
            targetTouchPos = Vec2(touchPos.x, m_character->m_torso->getBodySprite()->getPosition().y);

            // Set the velocity of the cannon body based on touch movement
            Vec2 touchDelta = touch->getDelta();
            b2Vec2 velocity(touchDelta.x, 0);
            m_character->m_torso->getBody()->SetLinearVelocity(velocity);
        }
    }
}

void GameScene::onTouchEnded(ax::Touch* touch, ax::Event* event)
{
    if (m_isTouchedOnCannon)
    {
        m_character->m_torso->getBody()->SetLinearVelocity(b2Vec2_zero);
        m_character->m_torso->getBody()->SetLinearDamping(10.0f);  // Adjust damping factor as needed
    }

    // Reset touch-related flags
    m_isTouched         = false;
    m_isTouchedOnCannon = false;
}

bool GameScene::isTouchingItem(ax::Touch* touch) const
{
    auto boundingBox = m_character->m_torso->getBodySprite()->getBoundingBox();
    return boundingBox.containsPoint(touch->getLocation());
}

void GameScene::renderDrawDebugger(b2World* world, bool isVisible)
{
    if (world)
    {
        m_b2dr = b2DebugRenderer::create(world);
        m_b2dr->setVisible(false);
        addChild(m_b2dr);
    }
}

void GameScene::createWorld(const b2Vec2& gravity)
{
    m_contactListener = std::make_unique<WorldContactListener>();
    m_world           = std::make_unique<b2World>(gravity);
    m_world->SetAllowSleeping(true);
    m_world->SetContinuousPhysics(true);
    m_world->SetContactListener(m_contactListener.get());
}

void GameScene::createWall()
{
    m_wall = std::make_unique<Wall>();
    m_wall->showOnGameScene({0, 0});
}

void GameScene::createBall()
{
    m_ball = std::make_unique<Ball>();
    m_ball->showOnGameScene({m_visibleSize.width * 0.5f, m_visibleSize.height * 0.6f});
}

void GameScene::createCharacter()
{
    // place on the ground
    b2Fixture* groundFixture{};
    for (b2Fixture* fixture = m_wall->getBody()->GetFixtureList(); fixture; fixture = fixture->GetNext())
    {
        if (fixture->GetUserData().pointer ==
            reinterpret_cast<uintptr_t>(reinterpret_cast<void*>(WallFixtureType::GROUND)))
        {
            groundFixture = fixture;
            break;
        }
    }

    // get the shape of the body
    float hx{};
    float hy{};

    if (b2PolygonShape const* groundShape = dynamic_cast<b2PolygonShape*>(groundFixture->GetShape()))
    {
        // Get the half-width (hx) and half-height (hy) of the ground shape.
        hx = groundShape->m_vertices[1].x;  // Assuming the shape is symmetric, this is half the width.
        hy = groundShape->m_vertices[2].y;  // Assuming the shape is symmetric, this is half the height.
    }

    // new
    auto otherBody          = groundFixture->GetBody();
    auto& positionOtherBody = otherBody->GetPosition();
    float angleOtherBody    = otherBody->GetAngle();
    b2Vec2 relativeOffset(hx / 2, hy);
    auto positionThisBody = positionOtherBody + b2Mul(b2Rot(angleOtherBody), relativeOffset);
    positionThisBody =
        positionThisBody - b2Mul(b2Rot(angleOtherBody), b2Vec2(0, -torso_NS::m_hY / GameUtils::PPM::kPpm));

    m_character = std::make_unique<Character>();
    m_character->createHumanBody(positionThisBody.x * GameUtils::PPM::kPpm, positionThisBody.y * GameUtils::PPM::kPpm);
    m_character->originPos = {positionThisBody.x * GameUtils::PPM::kPpm, positionThisBody.y * GameUtils::PPM::kPpm};
}

void GameScene::restartGame()
{
    // add back button on season selector
    auto m_restartbutton = Button::create("restart_win.png", "restart_win.png", "restart_win.png");
    m_restartbutton->setPosition(Vec2(m_visibleSize.width * 0.5f, m_visibleSize.height * 0.95f));
    m_restartbutton->setScale9Enabled(true);
    m_restartbutton->setScale(0.5f);
    m_restartbutton->addTouchEventListener([&](Ref*, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED)
        {
            this->restartGameWithTransition();
        }
    });
    addChild(m_restartbutton);
}

void GameScene::resetGame()
{
    // Pause the update function temporarily
    unschedule(AX_SCHEDULE_SELECTOR(GameScene::update));

    // Reset relevant variables or state
    _isUpdatePaused      = false;
    _isStartGame         = false;
    _isGameOver          = false;
    _isReviveGame        = false;
    _isGameOverHandled   = false;
    _isReviveGameHandled = false;
    setForRemoval        = false;
    timeScale            = 1.0f;
    m_eraseTimer         = 1.0f;
    _isWinGame           = false;
    _isWinGameHandled    = false;
    isCoinCollected      = false;
    isCoinRemoved        = false;
    _isShakeBush         = false;
    totalCoin            = 0;
    isItemsAllDetached   = false;

    // reset game state
    _gameState = GameState::init;

    // destroy the joints and body
    // Destroy any joints connected to bodies
    b2Joint* joint = m_world->GetJointList();
    while (joint)
    {
        b2Joint* nextJoint = joint->GetNext();
        m_world->DestroyJoint(joint);
        joint = nextJoint;
    }

    // Destroy bodies
    b2Body* body = m_world->GetBodyList();
    while (body)
    {
        b2Body* nextBody = body->GetNext();
        m_world->DestroyBody(body);
        body = nextBody;
    }

    // clean up contact listener
    m_contactListener = nullptr;

    // clean up world
    m_world = nullptr;

    // reset the set
    m_boardItems.clear();
    itemScheduledForRemoval.clear();

    // clear nodes add here
    this->removeAllChildren();

    //
    m_wall.reset();
    m_ball.reset();
    m_character.reset();
    m_board.reset();
    m_chain.reset();

    // create world
    createWorld(b2Vec2(0, physics::kGravity));

    // Recreate UI elements
    m_hud = Hud::create();
    m_hud->setGameSceneReference(this);
    addChild(m_hud, 99);

    // Recreate main menu layer
    m_mainMenu = MainMenuLayer::create();
    m_mainMenu->setGameSceneReference(this);
    addChild(m_mainMenu, 9999);

    // Recreate background
    auto bgSprite = Sprite::create("bg.png");
    bgSprite->setPosition(m_visibleSize.width / 2, m_visibleSize.height / 2);
    auto bgScaleX = m_visibleSize.width / bgSprite->getContentSize().width;
    auto bgScaleY = m_visibleSize.height / bgSprite->getContentSize().height;
    bgSprite->setScale(bgScaleX, bgScaleY);
    addChild(bgSprite);

    // Recreate b2DebugRenderer
    renderDrawDebugger(m_world.get(), true);

    // Recreate touch listener
    auto touchListener          = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = AX_CALLBACK_2(GameScene::onTouchBegan, this);
    touchListener->onTouchMoved = AX_CALLBACK_2(GameScene::onTouchMoved, this);
    touchListener->onTouchEnded = AX_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    // Recreate event listener for collisions
    auto eventListener = EventListenerCustom::create(COLLISION_EVENT_ID, [&](ax::EventCustom* event) {
        const auto* collisionPoint = static_cast<const Vec2*>(event->getUserData());
        // CLOG("hit ground");
        auto dust = DustParticle::create();
        dust->setupParticleProperties(*collisionPoint, "dirt_02.png");
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

    // Recreate schedule for box2d world update
    schedule(AX_SCHEDULE_SELECTOR(GameScene::update));

    startNewGame();
}

void GameScene::reviveGame()
{
    // reset game entities
    if (m_ball)
    {
        m_ball->destroyBody();
        // Remove Meteor from the scene
        this->removeChild(m_ball->getBodySprite());
        // m_ball->setPosition({ m_visibleSize.width / 2.0f, m_visibleSize.height * 0.7f });
    }
    m_ball.reset();

    // reset character to center of screen
    if (m_character)
    {
        // hair
        m_character->m_hair->destroyBody();
        this->removeChild(m_character->m_hair->getBodySprite());

        // head
        m_character->m_head->destroyBody();
        this->removeChild(m_character->m_head->getBodySprite());

        // torso
        m_character->m_torso->destroyBody();
        this->removeChild(m_character->m_torso->getBodySprite());

        // position
        // m_character->m_torso->setPosition(m_character->originPos);
    }
    m_character.reset();

    pauseGame();
}

void GameScene::pauseGame()
{
    if (!_isUpdatePaused)
    {
        _isUpdatePaused = true;

        // create a ball
        createBall();

        // create character
        createCharacter();

        // Additional logic for pausing the game (if needed)
        auto playButton = ui::Button::create("transparentDark40.png", "", "");
        playButton->setPosition(Vec2(m_visibleSize.width / 2.0f, m_visibleSize.height * 0.5f));
        playButton->setScale9Enabled(true);
        // playButton->setScale(0.5f);
        playButton->setSwallowTouches(true);
        addChild(playButton);

        playButton->addClickEventListener([=](Ref* sender) {
            resumeGame();
            // Remove the MainMenuLayer from its parent
            playButton->removeFromParentAndCleanup(true);
        });

        unschedule(AX_SCHEDULE_SELECTOR(GameScene::update));
    }
}

void GameScene::resumeGame()
{
    if (_isUpdatePaused)
    {
        _isUpdatePaused = false;

        schedule(AX_SCHEDULE_SELECTOR(GameScene::update));
    }
}

void GameScene::handleContactItemBall(Item* item)
{
    if (item && item->getNumContacts() == 1)
    {
        item->getBodySprite()->setColor(Color3B::GREEN);
    }
    else if (item && item->getNumContacts() == 2)
    {
        // Check if m1 is in meteorList
        // auto itemIt = std::find_if(m_boardItems.begin(), m_boardItems.end(),
        //                           [&](const std::unique_ptr<Item>& it) { return it.get() == item; });

        auto itemIt =
            std::ranges::find_if(m_boardItems, [&](const std::unique_ptr<Item>& it) { return it.get() == item; });

        // schedule it for removal
        if (itemIt != m_boardItems.end())
        {
            // item found in itemList, move it to itemScheduledForRemoval set
            itemScheduledForRemoval.insert(std::move(*itemIt));

            // Remove it from itemList
            m_boardItems.erase(itemIt);

            // set for removal
            setForRemoval = true;
        }
    }
}

void GameScene::createFadeLayer()
{
    fadeLayer = LayerColor::create(Color4B::WHITE);
    fadeLayer->setContentSize(Director::getInstance()->getVisibleSize());
    fadeLayer->setVisible(false);

    addChild(fadeLayer, 99);  // Add the layer on top of other nodes with a high z-order
}

void GameScene::restartGameWithTransition()
{
    // Pause the update function
    _isUpdatePaused = true;
    float duration  = 0.4f;
    fadeLayer->setOpacity(255);
    fadeLayer->setVisible(true);
    this->runAction(Sequence::create(DelayTime::create(duration),  // Wait for the fade-out to complete
                                     CallFunc::create([&]() {
                                         fadeLayer->setVisible(false);
                                         fadeLayer->removeFromParentAndCleanup(true);
                                         fadeLayer = nullptr;
                                         resetGame();
                                         _isUpdatePaused = false;
                                     }),
                                     nullptr));
}

void GameScene::startNewGame()
{
    // create enclosing wall
    createWall();

    // create ball
    createBall();

    // create character
    createCharacter();

    // the items to place on board
    createItems();
}

void GameScene::loadGame() {}

void GameScene::playGame()
{
    // ui
    m_hud->setVisible(true);

    //
    createRandomCoin();

    // chain
    clampChainToCeiling();

    // board
    clamplBoardToEndOfChain();
    placeItemsOnBoard();
}

void GameScene::onGameOver()
{
    timeScale = _isGameOver || _isWinGame ? 0.09f : 1.0f;

    // get mission label
    // CLOG("%s", std::string(m_hud->getMissionLabel()->getString()).c_str());
    /*GameOverLayer::LevelData newLevelData;
    newLevelData.currentLevel = "0";
    newLevelData.collectedCoins = "10";
    newLevelData.mission = std::string(m_hud->getMissionLabel()->getString());
    newLevelData.rewards = "500 points";*/

    // create revive layer
    // m_hud->setVisible(false);

    m_gOverLayer = GameOverLayer::create(std::string("a"));
    m_gOverLayer->setGameSceneReference(this);
    addChild(m_gOverLayer, 9999);
}

void GameScene::onGameWin()
{
    // AXLOG("Victory Game");

    // set
    timeScale = _isGameOver || _isWinGame ? 0.09f : 1.0f;

    // create revive layer
    m_victoryLayer = VictoryGameLayer::create(std::string("a"));
    m_victoryLayer->setGameSceneReference(this);
    addChild(m_victoryLayer, 190);
}

void GameScene::onReviveGame()
{
    // set
    // timeScale = _isGameOver || _isWinGame ? 0.09f : 1.0f;

    // create revive layer
    m_reviveLayer = ReviveGameLayer::create();
    m_reviveLayer->setGameSceneReference(this);
    addChild(m_reviveLayer, 190);
}

void GameScene::createRandomCoin()
{
    // Create a new Coin instance
    // Coin* coin = Coin::create();
    coin = Coin::create();

    // Random number generators
    std::random_device rd;
    std::mt19937 gen(rd());

    // Set a random x position within the specified range
    std::uniform_real_distribution<float> distX(m_visibleSize.width * 0.2f, m_visibleSize.width * 0.8f);
    float randomX = distX(gen);

    // Set a random y position within the specified range
    std::uniform_real_distribution<float> distY(m_visibleSize.height * 0.4f, m_visibleSize.height * 0.7f);
    float randomY = distY(gen);

    coin->setPosition(Vec2(randomX, randomY));

    // Add the coin to the scene
    addChild(coin);
}

void GameScene::collectCoin(ax::Sprite* coin, const ax::Vec2& targetPoint)
{
    // Move the coin to the target point with a MoveTo action
    float duration    = 0.25f;  // Adjust the duration as needed
    auto moveToAction = ax::MoveTo::create(duration, targetPoint);

    // You can also add other actions like fading out the coin
    auto fadeOutAction = ax::FadeOut::create(duration);

    ax::CallFunc* removeCoinAction = ax::CallFunc::create([this, coin]() {
        // Remove the coin from the scene after it reaches the target point
        coin->removeFromParentAndCleanup(true);

        // Update the coin label with the new total
        // int newTotalCoin = 1/* Calculate the new total coin */;
        m_hud->topContainer->updateCoinLabel(totalCoin);

        // Set isCoinRemoved to true
        isCoinRemoved = true;

        // Create a new coin after the previous one is removed
        // createRandomCoin();
    });

    // Create a sequence of actions
    auto sequence = ax::Sequence::create(moveToAction, fadeOutAction, removeCoinAction, nullptr);

    // Run the sequence on the coin sprite
    coin->runAction(sequence);
}

void GameScene::clampChainToCeiling()
{
    // ------------------------- get ceiling
    b2Fixture* ceilingFixture{};
    for (b2Fixture* fixture = m_wall->getBody()->GetFixtureList(); fixture; fixture = fixture->GetNext())
    {
        if (fixture->GetUserData().pointer ==
            reinterpret_cast<uintptr_t>(reinterpret_cast<void*>(WallFixtureType::CEILING)))
        {
            ceilingFixture = fixture;
            break;
        }
    }

    // get the shape of the body
    float hx{};
    float hy{};

    if (b2PolygonShape const* ceilingShape = dynamic_cast<b2PolygonShape*>(ceilingFixture->GetShape()))
    {
        // Get the half-width (hx) and half-height (hy) of the ground shape.
        hx = ceilingShape->m_vertices[1].x -
             ceilingShape->m_centroid.x;  // Assuming the shape is symmetric, this is half the width.
        hy = ceilingShape->m_vertices[2].y -
             ceilingShape->m_centroid.y;  // Assuming the shape is symmetric, this is half the height.
    }

    // Settings
    auto positionThisBody = b2Vec2(0, (m_visibleSize.height / GameUtils::PPM::kPpm) - hy / 2);

    // Create chain
    m_chain = std::make_unique<LinkedChain>();
    m_chain->showOnGameScene({m_visibleSize.width / 2, positionThisBody.y * GameUtils::PPM::kPpm});

    // join top chain to ceiling
    b2JointDef* jointDef;
    b2RevoluteJointDef revoluteJointDef;

    jointDef                        = &revoluteJointDef;
    revoluteJointDef.localAnchorA   = {(m_visibleSize.width / 2 / GameUtils::PPM::kPpm),
                                       (m_visibleSize.height / GameUtils::PPM::kPpm) - hy / 2};
    revoluteJointDef.localAnchorB   = {0, 0};
    revoluteJointDef.referenceAngle = 0.0f;
    revoluteJointDef.enableMotor    = false;
    revoluteJointDef.motorSpeed     = 0.0f;
    revoluteJointDef.maxMotorTorque = 0.0f;

    jointDef->bodyA             = ceilingFixture->GetBody();
    jointDef->bodyB             = m_chain->getChainEnds()[0];
    jointDef->collideConnected  = false;
    auto chainTop_ceiling_joint = m_world->CreateJoint(jointDef);
}

void GameScene::clamplBoardToEndOfChain()
{
    // get the end of chain
    auto last_chain    = m_chain->getChainEnds().back();
    auto& lastChainPos = last_chain->GetPosition();

    // create board
    m_board = std::make_unique<Board>();
    m_board->showOnGameScene({lastChainPos.x * GameUtils::PPM::kPpm, lastChainPos.y * GameUtils::PPM::kPpm});

    // link board to end of chain
    b2JointDef* jointDef;
    b2RevoluteJointDef revoluteJointDef;

    jointDef                        = &revoluteJointDef;
    revoluteJointDef.localAnchorA   = {0, 0};
    revoluteJointDef.localAnchorB   = {0, 0};
    revoluteJointDef.referenceAngle = 0.0f;
    revoluteJointDef.enableMotor    = false;
    revoluteJointDef.motorSpeed     = 0.0f;
    revoluteJointDef.maxMotorTorque = 0.0f;

    jointDef->bodyA            = last_chain;
    jointDef->bodyB            = m_board->getBody();
    jointDef->collideConnected = false;
    auto chainEnd_board_joint  = m_world->CreateJoint(jointDef);
}

void GameScene::createItems()
{
    int totalItems{4};
    for (int i = 0; i < totalItems; ++i)
    {
        auto item = std::make_unique<Fruit>(2);
        m_boardItems.push_back(std::move(item));
    }
}

void GameScene::placeItemsOnBoard()
{
    if (m_boardItems.empty())
    {
        return;
    }

    // number of items to be placed on the board
    auto numItems = static_cast<int>(m_boardItems.size());

    // Radius of the circular arrangement
    float circleRadius = m_board->getRadius() * 1.0f;

    // Calculate the angle between each ball
    auto angleIncrement = 2 * M_PI / numItems;

    for (int i = 0; i < numItems; i++)
    {
        // Calculate the position of the ball in the circular arrangement
        float angle = i * angleIncrement;
        float itemX = circleRadius * cosf(angle) / GameUtils::PPM::kPpm;
        float itemY = circleRadius * sinf(angle) / GameUtils::PPM::kPpm;

        m_boardItems[i]->originalPosition(itemX, itemY);
        // m_boardItems[i]->setPosition(Vec2(itemX, itemY));

        // get position of the board so you can place items on it
        auto& boardPos = m_board->getBody()->GetPosition();
        m_boardItems[i]->showOnGameScene({boardPos.x * GameUtils::PPM::kPpm + itemX * GameUtils::PPM::kPpm,
                                          boardPos.y * GameUtils::PPM::kPpm + itemY * GameUtils::PPM::kPpm});

        // Attach the ballBody to the circleShape
        b2WeldJointDef jointDef;
        jointDef.bodyA = m_board->getBody();  // The body with the circle shape
        jointDef.bodyB = m_boardItems[i]->getBody();
        jointDef.localAnchorA.Set(itemX, itemY);  // Position on the circle shape
        jointDef.localAnchorB.Set(0.0f, 0.0f);    // Center of the ball
        auto weldJoint = m_world->CreateJoint(&jointDef);
    }
}

void GameScene::updateItems(float dt)
{
    if (_isStartGame && !m_boardItems.empty())
    {
        for (auto const& items : m_boardItems)
        {
            if (items)
            {
                items->update(dt);
            }
        }
    }

    // update for meteor in the set
    if (_isStartGame && !itemScheduledForRemoval.empty())
    {
        for (auto& itemPtr : itemScheduledForRemoval)
        {
            auto item = itemPtr.get();
            // Check if the Meteor object is still valid
            if (item)
            {
                // Update the Meteor object
                item->update(dt);
            }
        }
    }
}

void GameScene::detachItemFromBoard(Item* item)
{
    // Find the corresponding weld joint attached to the ball body
    b2JointEdge* jointEdge = item->getBody()->GetJointList();
    while (jointEdge)
    {
        if (b2Joint* joint = jointEdge->joint; joint->GetType() == e_weldJoint &&
                                               joint->GetBodyA() == m_board->getBody() &&
                                               joint->GetBodyB() == item->getBody())
        {
            // Destroy the weld joint
            m_world->DestroyJoint(joint);
            break;
        }
        jointEdge = jointEdge->next;
    }
}

bool GameScene::onShakeBush()
{
    if (!_isShakeBush && GameUtils::areSpritesIntersecting(m_ball->getBodySprite(), m_wall->_bushSprite))
    {
        CLOG("bush n ball hit");

        // Define the shake duration and intensity
        float shakeIntensity = 5.0f;

        // Create a sequence of MoveBy actions to simulate the shake effect
        auto shakeSequence = Sequence::create(
            MoveBy::create(0.05f, Vec2(shakeIntensity, 0)), MoveBy::create(0.05f, Vec2(-2 * shakeIntensity, 0)),
            MoveBy::create(0.05f, Vec2(2 * shakeIntensity, 0)), MoveBy::create(0.05f, Vec2(-2 * shakeIntensity, 0)),
            MoveBy::create(0.05f, Vec2(2 * shakeIntensity, 0)), MoveBy::create(0.05f, Vec2(-shakeIntensity, 0)),
            nullptr);

        m_wall->_bushSprite->runAction(shakeSequence);

        _isShakeBush = true;
    }

    return _isShakeBush;
}

bool GameScene::onCollectCoin()
{
    if (_isStartGame && !_isGameOverHandled && !_isWinGameHandled)
    {
        if (!isCoinCollected && GameUtils::areSpritesIntersecting(m_ball->getBodySprite(), coin))
        {
            totalCoin += coin->getValue();
            Vec2 targetPoint = m_hud->topContainer->coinBtnWorldPos;
            collectCoin(coin, targetPoint);

            // play sound of collect coin
            Audio::getInstance().playSfx(std::string(sound_audio::coin_collect));

            isCoinCollected = true;
        }

        if (isCoinRemoved)
        {
            isCoinRemoved = false;
            this->scheduleOnce(
                [this](float dt) {
                isCoinCollected = false;
                createRandomCoin();
                },
                3.0f, "checkCoinRemoved");
        }
    }
    return isCoinCollected;
}

bool GameScene::onItemsAllDetached()
{
    // if number of items to be placed on board equals number of items set for removal
    if (auto totalItems{itemScheduledForRemoval.size()}; totalItems == 4 && !isItemsAllDetached)
    {
        isItemsAllDetached = true;
        this->scheduleOnce(
            [this](float dt) {
            this->_isWinGame = true;
            CLOG("removed");
            },
            1.0f, "ready4removal");
    }

    return isItemsAllDetached;
}

void GameScene::removeMeteor(float dt)
{
    if (!itemScheduledForRemoval.empty() && setForRemoval)
    {
        setForRemoval = false;

        // Process list for Meteor deletion
        auto meteorIt  = itemScheduledForRemoval.begin();
        auto meteorEnd = itemScheduledForRemoval.end();

        for (; meteorIt != meteorEnd; ++meteorIt)
        {
            auto& dyingMeteor = *meteorIt;

            // destroy its body
            // dyingMeteor->destroyBody();
            detachItemFromBoard(dyingMeteor.get());

            // Remove Meteor from the scene
            // this->removeChild(dyingMeteor->getBodySprite());
        }

        // Clear the list for the next time
        // itemScheduledForRemoval.clear();
    }
}
