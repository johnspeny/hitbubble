#pragma once

#include "axmol.h"
#include "ui/CocosGUI.h"
#include "Heart.h"
#include "TopContainer.h"

class GameScene;

class Hud : public ax::Layer
{
public:
	Hud();
	static Hud* create();
	bool init() override;
	void setGameSceneReference(GameScene* gameScene);

	// health

	// mission
	ax::Label* getMissionLabel()const { return missionLabel; };
	inline int getHits()const { return m_hits; };
	inline int getTotalHits()const { return m_totalHits; };
	inline void setHits(int count) { m_hits = count; };

	//void updateUI();
	void setCurrentMission(int mission_number, int hits, int totalHits);
	void updateCurrentMissionValues();

	// update
	void updateHearts(int health);

	TopContainer* topContainer;

private:
	ax::Size m_visibleSize;
	ax::Vec2 m_origin;

	// coins
	//int m_totalCoin{};
	//ax::Label* coinLabel{};
	//ax::ui::Layout* m_topContainer{};
	void displayMenuBar();

	// time | juggles | bursts
	int m_hits{};
	int m_totalHits{};
	ax::Label* missionLabel{};
	int m_currentMission{ 0 };
	void displayMission();
	std::unordered_map<int, std::string> missionFormats;

	GameScene* m_gameScene{};

	// hearts
	const int maxHearts{ 3 };
	const float heartSpacing{ 5.0f };
	void displayHearts();
	std::vector<Heart*> hearts;
};
