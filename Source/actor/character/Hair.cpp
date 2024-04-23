#include "Hair.h"

#include "GameUtils.h"
#include "Constants.h"
#include "helpers/Logger.h"
#include "scene/GameScene.h"
#include "scene/SceneManager.h"
#include "helpers/box2d/b2BodyBuilder.h"
#include "actor/character/Head.h"

using namespace category_bits;

namespace
{
	// Hair
	constexpr float kHairRadius = 40.0f; // 28
	constexpr auto kHairCategoryBits = kHair;
	constexpr auto kHairMaskBits = kWall | kBall | kWallLeft | kWallRight;

	// hair
	//constexpr auto kHairMaskBits = kWall | kBall | kWallLeft | kWallRight | kMeteor;
}

Hair::Hair(DynamicEntity* parent) :
	m_parent{ parent },
	m_radius{},
	m_scale{ 1.0f }
{
	GameUtils::PPM::initVars();
}

bool Hair::showOnGameScene(const Vec2& position)
{
	if (m_isShownOnGameScene)
	{
		return false;
	}

	m_isShownOnGameScene = true;

	// create the sprite
	m_bodySprite = Sprite::create("hair.png");

	// scale it down
	m_bodySprite->setScale(m_scale);
	//blinkToRed();

	defineBody(position.x, position.y);

	// set initial sprite position because it depends on another bodies position
	m_bodySprite->setPosition({ m_body->GetPosition().x * GameUtils::PPM::kPpm, m_body->GetPosition().y * GameUtils::PPM::kPpm });

	// add to this scene
	auto snmg = static_cast<GameScene*>(SceneManager::getInstance().getCurrentScene());
	snmg->addChild(m_bodySprite);

	return true;
}

void Hair::update(float dt)
{
	if (!m_body)
	{
		return;
	}

	DynamicEntity::update(dt);
}

void Hair::blinkToRed()
{
	/**************************************************************************/

	// Save the original color of the sprite
	Color3B originalColor = m_bodySprite->getColor();

	// Create an action to change the color to red
	ActionInterval* turnRed = TintTo::create(0.2f, 255, 0, 0);

	// Create an action to revert the color to the original color
	ActionInterval* turnBack = TintTo::create(0.2f, originalColor.r, originalColor.g, originalColor.b);

	// Create a sequence of actions to blink
	Sequence* blinkSequence = Sequence::create(turnRed, turnBack, nullptr);

	// Repeat the blink sequence for 3 seconds
	Repeat* blinkRepeat = Repeat::create(blinkSequence, 3);

	// Run the blink action
	m_bodySprite->runAction(blinkRepeat);

	// Delay for 3 seconds before stopping the blinking
	DelayTime* delay = DelayTime::create(3.0f);

	// Create a FiniteTimeAction to stop the blinking
	FiniteTimeAction* stopBlinking = CallFunc::create([this]() {
		m_bodySprite->stopAllActions();
		});

	/**************************************************************************/
}

void Hair::defineBody(float x, float y)
{
	auto snMgr = SceneManager::getInstance().getCurrentScene<GameScene>();

	// m_radius = 20.0f;
	m_radius = kHairRadius / GameUtils::PPM::kPpm;

	// new
	auto otherBody = dynamic_cast<Head*>(m_parent);
	auto& positionOtherBody = otherBody->getBody()->GetPosition();
	float angleOtherBody = otherBody->getBody()->GetAngle();
	b2Vec2 relativeOffset(0, otherBody->getRadius() * 0.8f / GameUtils::PPM::kPpm);
	auto positionThisBody = positionOtherBody + b2Mul(b2Rot(angleOtherBody), relativeOffset);
	//positionThisBody = positionThisBody - b2Mul(b2Rot(angleOtherBody), b2Vec2(0, m_radius / GameUtils::PPM::kPpm));

	// new body
	// Create a body definition
	b2BodyDef boxBodyDef;
	boxBodyDef.type = b2_dynamicBody; // Adjust the body type as needed
	boxBodyDef.position.Set(positionThisBody.x, positionThisBody.y); // Set the initial position
	// Create a Box2D body
	m_body = snMgr->getWorld()->CreateBody(&boxBodyDef);

	//getFixtureDefsFromPhysicsEditor("hair.plist", m_body, m_scale * 0.9f);

	const float hairRadius = (m_radius); // Define the hair segment's radius

	const int numSegments = 8; // Number of segments for the semi-circle

	float yOffset = hairRadius * 0.8f;

	std::vector<b2Vec2> hairVertices;
	for (int i = numSegments / 4; i <= (numSegments * 3) / 4; ++i) {
		float angle = b2_pi * i / static_cast<float>(numSegments);
		float xh = hairRadius * std::cos(angle);
		float yh = hairRadius * std::sin(angle) - yOffset;
		hairVertices.push_back(b2Vec2(xh, yh));
	}

	// Create a b2PolygonShape object
	b2PolygonShape hairShape;

	// Populate the shape with the circular segment vertices
	b2Vec2* segmentVertices = hairVertices.data();
	int numVertices = hairVertices.size();
	hairShape.Set(segmentVertices, numVertices);

	// Create a fixture definition
	b2FixtureDef hairFixtureDef;
	hairFixtureDef.shape = &hairShape;
	hairFixtureDef.density = 1.0f; // Set density according to your needs
	hairFixtureDef.friction = 0.1f; // Set density according to your needs
	hairFixtureDef.filter.categoryBits = kHairCategoryBits;
	hairFixtureDef.filter.maskBits = kHairMaskBits;

	// Attach the hair fixture to the circle body
	m_body->CreateFixture(&hairFixtureDef);
}

void Hair::getFixtureDefsFromPhysicsEditor(const std::string& name, b2Body* new_body, float scaleFactor)
{
	auto fileUtils = FileUtils::getInstance();
	auto dict = fileUtils->getValueMapFromFile(name);
	if (dict.empty())
	{
		// plist file not found
		return;
	}

	ValueMap& metadata = dict["metadata"].asValueMap();
	int format = metadata["format"].asInt();
	if (format != 1)
	{
		CCASSERT(format == 1, "format not supported!");
		return;
	}

	ValueMap& bodydict = dict.at("bodies").asValueMap();

	for (auto iter = bodydict.cbegin(); iter != bodydict.cend(); ++iter)
	{
		const ValueMap& bodyData = iter->second.asValueMap();
		std::string bodyName = iter->first;

		const ValueVector& fixtureList = bodyData.at("fixtures").asValueVector();
		for (auto& fixtureitem : fixtureList)
		{
			auto& fixturedata = fixtureitem.asValueMap();
			std::string fixtureType = fixturedata.at("fixture_type").asString();

			if (fixtureType == "POLYGON")
			{
				const ValueVector& polygonsArray = fixturedata.at("polygons").asValueVector();

				for (const auto& polygonitem : polygonsArray)
				{
					auto& polygonArray = polygonitem.asValueVector();
					int pcount = (int)polygonArray.size();
					b2Vec2* vertices = new b2Vec2[pcount];
					int vindex = 0;
					for (auto& pointString : polygonArray)
					{
						auto offset = PointFromString(pointString.asString());
						vertices[vindex].x = offset.x * scaleFactor / GameUtils::PPM::kPpm;
						vertices[vindex].y = offset.y * scaleFactor / GameUtils::PPM::kPpm;
						//vertices[vindex].Set(offset.x * scaleFactor / GameUtils::PPM::kPpm, offset.y * scaleFactor / GameUtils::PPM::kPpm);
						vindex++;
					}

					// polygon
					b2PolygonShape polygonShape;
					polygonShape.Set(vertices, vindex);

					// shape properties
					b2FixtureDef myFixtureDef;//
					myFixtureDef.shape = &polygonShape;
					myFixtureDef.density = 0.0000001f;
					myFixtureDef.friction = 0.1f;
					myFixtureDef.filter.categoryBits = kHairCategoryBits;
					myFixtureDef.filter.maskBits = kHairMaskBits;
					//myFixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

					auto fixture = new_body->CreateFixture(&myFixtureDef);
					delete[] vertices;
				}
			}
			else if (fixtureType == "CIRCLE")
			{
				const ValueMap& circleData = fixturedata.at("circle").asValueMap();
				Point p = PointFromString(circleData.at("position").asString()) * scaleFactor / GameUtils::PPM::kPpm;

				// circle
				b2CircleShape circleShape;
				circleShape.m_radius = circleData.at("radius").asFloat() * scaleFactor / GameUtils::PPM::kPpm;
				circleShape.m_p = b2Vec2(p.x, p.y);

				// shape properties
				b2FixtureDef myFixtureDef;
				myFixtureDef.shape = &circleShape;
				myFixtureDef.density = 1.0f;
				myFixtureDef.friction = 0.1f;
				myFixtureDef.filter.categoryBits = kHairMaskBits;
				myFixtureDef.filter.maskBits = kHairMaskBits;
				//myFixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

				auto fixture = new_body->CreateFixture(&myFixtureDef);
			}
		}
	}
}

b2Body* Hair::createCircularSegment(b2World* world, float radius, float startAngle, float endAngle,
	b2Vec2 position, float ppm) {
	return nullptr;
}
