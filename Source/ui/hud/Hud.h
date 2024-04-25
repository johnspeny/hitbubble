#pragma once

#include "axmol.h"
#include "ui/CocosGUI.h"
#include "TopContainer.h"

class GameScene;

class Hud : public ax::Layer
{
public:
    Hud();
    static Hud* create();
    bool init() override;
    void setGameSceneReference(GameScene* gameScene);
    TopContainer* topContainer;

private:
    ax::Size m_visibleSize;
    ax::Vec2 m_origin;
    void displayMenuBar();
    GameScene* m_gameScene{};
};
