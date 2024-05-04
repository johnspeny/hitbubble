#pragma once

#include "axmol.h"
#include "helpers/box2d/b2DebugRenderer.h"
#include "box2d/box2d.h"
#include "WorldContactListener.h"
#include "entity/Wall.h"
#include "entity/LinkedChain.h"
#include "entity/Board.h"
#include "actor/character/Character.h"
#include "actor/Ball.h"
#include "ui/hud/Hud.h"
#include "layers/MainMenuLayer.h"
#include "layers/GameOverLayer.h"
#include "layers/VictoryGameLayer.h"
#include "layers/ReviveGameLayer.h"
#include "actor/Coin.h"
#include "actor/item/Item.h"
#include "actor/item/Fruit.h"

class GameScene : public ax::Scene
{
public:
    enum class GameState
    {
        init = 0,
        update,
        pause,
        loading,
        settings,
        revive,
        gameover,
        victory,
        shop,
        mainmenu,
        end,
    };

    bool _isUpdatePaused;
    bool _isStartGame;
    bool _isGameOver;
    bool _isWinGame;
    bool _isReviveGame;
    bool _isGameOverHandled{false};
    bool _isWinGameHandled{false};
    bool _isReviveGameHandled{false};
    bool _isShakeBush{false};

    GameScene();
    ~GameScene() override;
    static GameScene* create();
    bool init() override;
    void update(float dt) override;
    inline b2World* getWorld() const { return m_world.get(); }

    // game options
    void startNewGame();
    void playGame();
    void loadGame();
    void resetGame();
    void reviveGame();
    void pauseGame();
    void resumeGame();

    Hud* m_hud;  // autorelease

    //
    void handleContactItemBall(Item* item);
    void removeMeteor(float dt);

    void setCurrentIndices(int currentSeasonIndex,
                           int currentLevelIndex,
                           int totalItems,
                           const std::vector<int>& points);

private:
    GameState _gameState = GameState::init;

    int m_currentSeasonIndex;
    int m_currentLevelIndex;
    int m_totalItems;
    std::vector<int> m_points;

    ax::Size m_visibleSize;
    ax::Vec2 m_origin;
    float fixedTimeStep{1.0f / 60.0f};

    bool setForRemoval{false};
    float m_eraseTimer{1.0f};
    float timeScale;

    MainMenuLayer* m_mainMenu;         // autorelease object
    GameOverLayer* m_gOverLayer;       // autorelease
    VictoryGameLayer* m_victoryLayer;  // autorelease
    ReviveGameLayer* m_reviveLayer;    // autorelease

    b2DebugRenderer* m_b2dr;  // autorelease object
    std::unique_ptr<b2World> m_world;
    std::unique_ptr<WorldContactListener> m_contactListener;

    void renderDrawDebugger(b2World* world, bool isVisible);
    void createWorld(const b2Vec2& gravity);
    void updatePhysicsWorld(float dt);

    void updatePlayer(float dt);
    void updateBall(float dt);

    // create a wall
    std::unique_ptr<Wall> m_wall;
    void createWall();

    // create a ball
    std::unique_ptr<Ball> m_ball;
    void createBall();

    // create a player/character
    std::unique_ptr<Character> m_character;
    void createCharacter();

    // restart the game
    ax::LayerColor* fadeLayer;
    void restartGame();
    void createFadeLayer();
    void restartGameWithTransition();

    // swipe functionality
    ax::Vec2 initialTouchPos;
    ax::Vec2 targetTouchPos;
    bool m_isTouchedOnCannon{false};
    bool m_isTouched{false};

    // touch
    bool onTouchBegan(ax::Touch* touch, ax::Event* event);
    void onTouchMoved(ax::Touch* touch, ax::Event* event);
    void onTouchEnded(ax::Touch* touch, ax::Event* event);
    bool isTouchingItem(ax::Touch* touch) const;

    void onGameOver();
    void onGameWin();
    void onReviveGame();

    // coin
    Coin* coin;
    void createRandomCoin();
    void collectCoin(ax::Sprite* coin, const ax::Vec2& targetPoint);
    bool isCoinCollected{false};
    int totalCoin{0};
    bool isCoinRemoved{false};

    // chain
    void clampChainToCeiling();
    std::unique_ptr<LinkedChain> m_chain;

    // board
    std::unique_ptr<Board> m_board;
    void clamplBoardToEndOfChain();
    void placeItemsOnBoard();
    void createItems();
    std::vector<std::unique_ptr<Item>> m_boardItems;
    std::set<std::unique_ptr<Item>> itemScheduledForRemoval;
    void updateItems(float dt);
    void detachItemFromBoard(Item* item);
    bool isItemsAllDetached{false};

    // other
    bool onShakeBush();
    bool onCollectCoin();
    bool onItemsAllDetached();
};
