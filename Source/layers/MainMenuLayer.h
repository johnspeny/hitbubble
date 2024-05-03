#pragma once

#include "axmol.h"
#include "ui/CocosGUI.h"
#include "season/Season.h"

class GameScene;

class MainMenuLayer : public ax::LayerColor
{
private:
    ax::Size m_visibleSize;
    ax::Vec2 m_origin;
    int m_totalCoin{};

    ax::ui::ImageView* m_touchBgImage;

    MainMenuLayer();
    GameScene* m_gameScene{};

    bool m_swallowTouches;
    void updateSwallowTouches();
    ax::EventListenerTouchOneByOne* m_touchListener;

    bool onTouchBegan(ax::Touch* touch, ax::Event* event);

    // seasons
    int m_currentSeasonIndex;
    int m_currentLevelIndex;
    int m_totalSeasons{};
    std::vector<std::unique_ptr<Season>> m_seasons;
    void loadSeasonFromJsonFile();
    void createSeason();

public:
    static MainMenuLayer* create();
    bool init() override;

    void setGameSceneReference(GameScene* gameScene);
};
