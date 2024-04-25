#include "Hud.h"
#include "Constants.h"
#include "scene/GameScene.h"
#include "helpers/Logger.h"

USING_NS_AX;

Hud::Hud()
{
    m_origin      = Director::getInstance()->getVisibleOrigin();
    m_visibleSize = Director::getInstance()->getVisibleSize();
}

Hud* Hud::create()
{
    auto pRet = new Hud();
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

bool Hud::init()
{
    if (!Layer::init())
        return false;

    setVisible(true);

    displayMenuBar();

    return true;
}

void Hud::setGameSceneReference(GameScene* gameScene)
{
    m_gameScene = gameScene;
}

void Hud::displayMenuBar()
{
    topContainer = TopContainer::create();
    addChild(topContainer);
}
