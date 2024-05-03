#pragma once

#include "axmol.h"
#include "ui/CocosGUI.h"

class GameScene;

class VictoryGameLayer : public ax::Layer
{
public:
    static VictoryGameLayer* create(int seasonId, int levelId, int coin);
    bool init() override;
    void setGameSceneReference(GameScene* gameScene);

private:
    ax::Size m_visibleSize;
    ax::Vec2 m_origin;
    int m_seasonIndex;
    int m_levelIndex;

    // data
    int m_collectedCoins;
    std::string m_rewards;

    VictoryGameLayer(int seasonId, int levelId, int coin);
    GameScene* m_gameScene;
    ax::Layer* overlayLayer;
    ax::ui::ImageView* bg;

    bool onTouchBegan(ax::Touch* touch, ax::Event* event);
    void displayCurrentLevel();
    void goToNextLevel();
};
