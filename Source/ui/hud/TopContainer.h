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
	ax::Vec2 coinBtnWorldPos;

private:
	ax::Label* coinLabel;
	ax::Size m_visibleSize;
	ax::Vec2 m_origin;

	void addHomeButton();
	void addCoinButton();
	void addLevelDisplay();
};