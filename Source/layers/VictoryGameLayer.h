#pragma once

#include "axmol.h"
#include "ui/CocosGUI.h"

class GameScene;

class VictoryGameLayer : public ax::Layer
{
public:
    static VictoryGameLayer* create(const std::string& level, int coin);
    bool init() override;
    void setGameSceneReference(GameScene* gameScene);

private:
    ax::Size m_visibleSize;
    ax::Vec2 m_origin;
    int m_seasonIndex;
    int m_levelIndex;

    // data
    std::string m_currentLevel;
    int m_collectedCoins;
    std::string m_rewards;

    VictoryGameLayer(const std::string& m_level, int coin);
    GameScene* m_gameScene;
    ax::Layer* overlayLayer;
    ax::ui::ImageView* bg;

    bool onTouchBegan(ax::Touch* touch, ax::Event* event);
    void displayCurrentLevel();
    void goToNextLevel();
};
