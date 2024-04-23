#pragma once

#include "axmol.h"
#include "ui/UILayout.h"
#include "ui/CocosGUI.h"

class ProgressUI final
{
private:
	const int m_minTackle;
	int m_maxJuggle;
	ax::Sprite* m_bgImg;
	ax::ui::LoadingBar* m_fgImg;
	ax::ui::Layout* m_layout;

	ax::Label* leftTackleLabel;
	ax::Label* rightTackleLabel;

	ax::Sprite* leftTackleSprite;
	ax::Sprite* rightTackleSprite;

public:
	ProgressUI(const std::string& bgImgPath, const std::string& fgImgPath, const int maxJuggle);

	inline int getMaxJuggleForCurrentLevel()const { return m_maxJuggle; }
	inline int getPercentForProgressUI() const { return static_cast<int>(m_fgImg->getPercent()); }
	inline void setPercentForProgressUI(float percent) { m_fgImg->setPercent(percent); }
	inline ax::ui::Layout* getLayout() const { return m_layout; }
};