#pragma once

#include "axmol.h"

class SplashScene : public ax::Scene
{
private:
	ax::Size m_visibleSize;
	ax::Vec2 m_origin;
	int numOfResLoaded;
	ax::Sprite* logoSprite;

	void loadResources();
	void updateResources(float dt);
	void loadSpriteFrames(std::string_view file);
	void displayLogo();
	void displayLogoText();
	void startLoop(float dt);

public:
	SplashScene();

	// init the screen
	bool init() override;
};
