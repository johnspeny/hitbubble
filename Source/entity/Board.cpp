#include "Board.h"
#include "scene/GameScene.h"
#include "scene/SceneManager.h"
#include "helpers/box2d/b2BodyBuilder.h"
#include "GameUtils.h"
#include "Constants.h"

using namespace category_bits;

namespace {
	constexpr auto kBoardCategoryBits = kBoard;
	constexpr auto kBoardMaskBits = 0;
}

Board::Board() :
	m_radius{ 50.0f }
{
	GameUtils::PPM::initVars();
}

bool Board::showOnGameScene(const Vec2& position)
{
	if (m_isShownOnGameScene)
	{
		return false;
	}

	m_isShownOnGameScene = true;

	// create the sprite
	m_bodySprite = Sprite::create("circle.png");

	// set initial sprite position
	m_bodySprite->setPosition(position);

	defineBody(position.x, position.y);

	// scale it down
	float scale = m_radius / (m_bodySprite->getContentSize().width / 2.0f);
	m_bodySprite->setScale(scale);

	// add to this scene
	auto snMgr = SceneManager::getInstance().getCurrentScene<GameScene>();
	snMgr->addChild(m_bodySprite);

	return true;
}

void Board::update(float dt)
{
	if (!m_body)
	{
		return;
	}

	DynamicEntity::update(dt);
    // keep the board rotating even when other bodies impede it
    m_body->ApplyTorque(0.4f, false);
}

void Board::defineBody(float x, float y)
{
	// change direction of rotation at start of game
	auto rand = AXRANDOM_MINUS1_1() * 2 + 1;
	rand = generateRandomAngular(2, 4);

	auto snMgr = SceneManager::getInstance().getCurrentScene<GameScene>();

	b2BodyBuilder bodyBuilder(snMgr->getWorld());

	m_body = bodyBuilder.type(b2_dynamicBody)
		.angularVelocity(rand)
		.position(x, y, GameUtils::PPM::kPpm)
		.buildBody();

	//m_radius = m_bodySprite->getContentSize().width / 2.0f;

	bodyBuilder.newCircleFixture(m_radius, GameUtils::PPM::kPpm)
		.denstity(1.0f)
		.friction(0.0f)
		.restitution(0.0f)
		.categoryBits(kBoardCategoryBits)
		.maskBits(kBoardMaskBits)
		.buildFixture();
}

int Board::generateRandomAngular(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	// Randomly select a sprite name
	std::uniform_int_distribution<> dist(min, max);
	int randomAngularSpeed = dist(gen);

	// random sign
	// Initialize the random number generator with a seed
	std::srand(std::time(nullptr));

	// Generate a random number between 0 and 1
	int randomValue = std::rand() % 2;

	// Map the 0 to -1 and 1 to 1
	int result = (randomValue == 0) ? -1 : 1;

	return randomAngularSpeed * result;
}

const std::string& Board::selectRandomSpriteName(const std::vector<std::string>& spriteNames) const
{
	// Random number generator
	std::random_device rd;
	std::mt19937 gen(rd());

	// Randomly select a sprite name
	std::uniform_int_distribution<> dist(0, static_cast<int>(spriteNames.size()) - 1);
	int randomIndex = dist(gen);
	const std::string& randomSpriteName = spriteNames[randomIndex];
	return randomSpriteName;
}