#pragma once

#include "axmol.h"
#include "ui/CocosGUI.h"

class GameScene;

class VictoryGameLayer : public ax::Layer
{
public:
	static VictoryGameLayer* create(const std::string& m_mission);
	bool init() override;
	void setGameSceneReference(GameScene* gameScene);

private:
	ax::Size m_visibleSize;
	ax::Vec2 m_origin;

	// data
	std::string m_currentLevel;
	std::string m_collectedCoins;
	std::string m_mission;
	std::string m_rewards;

	VictoryGameLayer(const std::string& _mission);
	GameScene* m_gameScene;
	ax::Layer* overlayLayer;
	ax::ui::ImageView* bg;

	bool onTouchBegan(ax::Touch* touch, ax::Event* event);
	void displayCurrentLevel();
	void displayCurrentMission();
};
