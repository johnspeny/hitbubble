#include "Hud.h"
#include "Constants.h"
#include "scene/GameScene.h"
#include "helpers/Logger.h"

USING_NS_AX;

Hud::Hud()
{
	m_origin = Director::getInstance()->getVisibleOrigin();
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

	setVisible(false);

	displayMenuBar();

	displayHearts();

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

void Hud::displayMission()
{
	if (missionFormats.empty()) {
		missionFormats[1] = "Heads: %d/%d";
		missionFormats[2] = "Balls: %d/%d";
		missionFormats[3] = "Goals: %d/%d";
	}

	std::string targetString = StringUtils::format(missionFormats[m_currentMission].c_str(), m_hits, m_totalHits);

	missionLabel = Label::createWithBMFont(fonts::hugmat_fnt, targetString);
	missionLabel->setBMFontSize(18.0f);
	missionLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	missionLabel->setPosition({ m_visibleSize.width * 0.5f, m_visibleSize.height * 0.9f });
	missionLabel->setColor(Color3B::BLUE);
	addChild(missionLabel);
}

void Hud::displayHearts()
{
	auto m_lifeContainer = Layout::create();
	m_lifeContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	m_lifeContainer->setContentSize(Size(m_visibleSize.width * 0.1f, m_visibleSize.height * 0.1f));
	m_lifeContainer->setPosition(Vec2(m_origin.x + m_visibleSize.width * 0.09f, m_visibleSize.height - topContainer->getContentSize().height * 1.0f));
	m_lifeContainer->setLayoutType(Layout::Type::ABSOLUTE);
	//m_lifeContainer->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	//m_lifeContainer->setBackGroundColor(Color3B::BLUE);
	addChild(m_lifeContainer);

	auto totalHeightOfHearts = 0;
	for (int i = 0; i < maxHearts; ++i) {
		auto heart = Heart::create();
		float scale = m_lifeContainer->getContentSize().width / heart->getContentSize().width * 0.6f;
		heart->setScale(scale);
		m_lifeContainer->addChild(heart);
		hearts.push_back(heart);

		// add the heights
		totalHeightOfHearts += heart->getContentSize().height * scale;
	}

	float containerHeight = m_lifeContainer->getContentSize().height;
	float spacing = (containerHeight - totalHeightOfHearts) / 5;
	float yPosition = spacing;

	for (auto& heart : hearts)
	{
		float xPosition = m_lifeContainer->getContentSize().width / 2;
		heart->setPosition(xPosition, yPosition + heart->getContentSize().height * heart->getScale() / 2);
		yPosition += heart->getContentSize().height * heart->getScale() + spacing + 2;
	}
}

void Hud::updateHearts(int health)
{
	// Update hearts based on character's health
	for (int i = 0; i < maxHearts; ++i) {
		if (i < health) {
			hearts[i]->setFilled(true);
		}
		else {
			hearts[i]->setFilled(false);
		}
	}
}

void Hud::setCurrentMission(int mission_number, int hits, int totalHits)
{
	m_currentMission = mission_number;
	m_hits = hits;
	m_totalHits = totalHits;

	displayMission();
}

void Hud::updateCurrentMissionValues()
{
	if (m_hits < m_totalHits)
	{
		m_hits++;
		//CLOG("updating ui %d", m_hits);
		missionLabel->setString(StringUtils::format(missionFormats[m_currentMission].c_str(), m_hits, m_totalHits));
		if (m_hits == m_totalHits) {
			missionLabel->setColor(Color3B::GREEN);
			m_gameScene->_isWinGame = true;
		}
	}
}