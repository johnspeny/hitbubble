#include "ProgressUI.h"

USING_NS_AX;

using namespace ui;

ProgressUI::ProgressUI(const std::string& bgImgPath, const std::string& fgImgPath, const int maxJuggle) :
	m_minTackle{ 0 },
	m_maxJuggle{ maxJuggle },
	m_bgImg{ Sprite::create(bgImgPath) },
	m_fgImg{ LoadingBar::create(fgImgPath) },
	m_layout{ ax::ui::Layout::create() }
{
	m_fgImg->setDirection(LoadingBar::Direction::LEFT);
	m_fgImg->setPercent(0);

	// sprite
	leftTackleSprite = Sprite::create("CloseNormal.png");
	rightTackleSprite = Sprite::create("CloseNormal.png");

	leftTackleSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	rightTackleSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

	leftTackleSprite->setPositionX(0 - m_bgImg->getContentSize().width / 2);
	rightTackleSprite->setPositionX(m_bgImg->getContentSize().width / 2);

	// labels
	leftTackleLabel = Label::createWithBMFont("fonts/enligsh-chinese.fnt", std::to_string(m_minTackle));
	rightTackleLabel = Label::createWithBMFont("fonts/enligsh-chinese.fnt", std::to_string(m_maxJuggle));

	leftTackleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	rightTackleLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	rightTackleLabel->setPositionX(rightTackleSprite->getContentSize().width / 2);
	rightTackleLabel->setPositionY(rightTackleSprite->getContentSize().height / 2);

	leftTackleLabel->setPositionX(leftTackleSprite->getContentSize().width / 2);
	leftTackleLabel->setPositionY(leftTackleSprite->getContentSize().height / 2);

	// add to layout
	m_layout->addChild(leftTackleSprite);
	m_layout->addChild(rightTackleSprite);

	rightTackleSprite->addChild(rightTackleLabel);
	leftTackleSprite->addChild(leftTackleLabel);

	m_layout->addChild(m_bgImg);
	m_layout->addChild(m_fgImg);
}