#pragma once

#include "axmol.h"
#include "ui/CocosGUI.h"

class TopContainer : public ax::ui::Layout
{
public:
    TopContainer();
    ~TopContainer() = default;

    static TopContainer* create();

    bool init() override;

    void updateCoinLabel(int totalCoin);
    void updateLevelLabel(int level);
    ax::Vec2 coinBtnWorldPos;
    void setGameLevel(int level);
    int getGameLevel() const;

private:
    ax::Label* coinLabel;
    ax::Label* levelLabel;
    ax::Size m_visibleSize;
    ax::Vec2 m_origin;
    int m_gameLevel{0};
    int m_totalCoin{0};

    void addHomeButton();
    void addCoinButton();
    void addLevelDisplay();
};
