#pragma once

#include "axmol.h"
#include "ui/CocosGUI.h"

class GameScene;

class ReviveGameLayer : public ax::Layer
{
public:
	static ReviveGameLayer* create();
	bool init() override;
	void setGameSceneReference(GameScene* gameScene);

private:
	ax::Size m_visibleSize;
	ax::Vec2 m_origin;

	// Add member variables to your class for countdown-related values
	int countdownSeconds{ 10 };
	int currentCountdown{};
	void showGameOverLayer();
	ax::ui::Button* retryButton;

	ReviveGameLayer();
	GameScene* m_gameScene;
	ax::Layer* overlayLayer;
	ax::ui::ImageView* bg;

	bool onTouchBegan(ax::Touch* touch, ax::Event* event);
};
