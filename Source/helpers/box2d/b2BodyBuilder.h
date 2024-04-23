#ifndef __B2BODYBUILDER_H__
#define __B2BODYBUILDER_H__

#define _USE_MATH_DEFINES
#include <cmath>

#include "box2d/box2d.h"
#include <memory>

class b2BodyBuilder
{
public:
	explicit b2BodyBuilder(b2World* world);
	explicit b2BodyBuilder(b2Body* body);
	explicit b2BodyBuilder(const b2BodyBuilder& otherb2BodyBuilder);
	b2BodyBuilder& operator=(const b2BodyBuilder& otherb2BodyBuilder);

	virtual ~b2BodyBuilder() = default;

	b2BodyBuilder& type(b2BodyType bodyType);
	b2BodyBuilder& angle(float angle);
	b2BodyBuilder& angularDamping(float value);
	b2BodyBuilder& awake(bool value);
	b2BodyBuilder& linearDamping(float value);
	b2BodyBuilder& rotation(bool isFixed);
	b2BodyBuilder& angularVelocity(float velocity);
	b2BodyBuilder& position(b2Vec2 position, float ppm);
	b2BodyBuilder& position(float x, float y, float ppm);

	b2BodyBuilder& newRectangleFixture(float hx, float hy, float ppm);
	b2BodyBuilder& newRectangleFixture(float hx, float hy, const b2Vec2& center, float angle, float ppm);
	b2BodyBuilder& newPolygonFixture(const b2Vec2* vertices, size_t count, float ppm);
	b2BodyBuilder& newPolylineFixture(const b2Vec2* vertices, size_t count, float ppm);
	b2BodyBuilder& newEdgeShapeFixture(const b2Vec2& vertex1, const b2Vec2& vertex2, float ppm);
	b2BodyBuilder& newCircleFixture(float radius, float ppm);
	b2BodyBuilder& newCircleFixture(float radius, float scale, float ppm);
	b2BodyBuilder& newCircleFixture(const b2Vec2& centerPos, float radius, float ppm);

	b2BodyBuilder& categoryBits(short categoryBits);
	b2BodyBuilder& maskBits(short maskBits);
	b2BodyBuilder& setSensor(bool isSensor);
	b2BodyBuilder& friction(float friction);
	b2BodyBuilder& groupIndex(int index);
	b2BodyBuilder& denstity(float denstity);
	b2BodyBuilder& restitution(float restitution);
	b2BodyBuilder& setUserData(void* userData);

	b2Fixture* buildFixture();
	b2Body* buildBody();

private:
	b2World* _world;
	b2Body* _body;
	b2BodyDef _bdef;
	std::unique_ptr<b2Shape> _shape;
	b2FixtureDef _fdef;
	void* _userData;
	b2Fixture* _fixture;
};

#endif // !__B2BODYBUILDER_H__
