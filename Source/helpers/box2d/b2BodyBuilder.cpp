#include "b2BodyBuilder.h"

b2BodyBuilder::b2BodyBuilder(b2World* world) :
	_world(world),
	_body(),
	_shape(),
	_fixture(),
	_userData()
{
}

b2BodyBuilder::b2BodyBuilder(b2Body* body) :
	_body(body),
	_world(body->GetWorld()),
	_shape(),
	_fixture(),
	_userData()
{
}

b2BodyBuilder::b2BodyBuilder(const b2BodyBuilder& otherb2BodyBuilder) :
	_world(otherb2BodyBuilder._world),
	_body(otherb2BodyBuilder._body),
	_fixture(otherb2BodyBuilder._fixture),
	_shape(otherb2BodyBuilder._shape.get()),
	_userData(otherb2BodyBuilder._userData)
{
}

b2BodyBuilder& b2BodyBuilder::operator=(const b2BodyBuilder& otherb2BodyBuilder)
{
	// // O: insert return statement here
	_world = otherb2BodyBuilder._world;
	_body = otherb2BodyBuilder._body;
	_fixture = otherb2BodyBuilder._fixture;
	_shape = std::unique_ptr<b2Shape>(otherb2BodyBuilder._shape.get());
	_userData = otherb2BodyBuilder._userData;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::type(b2BodyType bodyType)
{
	// // O: insert return statement here
	_bdef.type = bodyType;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::angle(float angle)
{
	// TODO: insert return statement here

	_bdef.angle = angle * M_PI / 180.0f;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::angularDamping(float value)
{
	// TODO: insert return statement here
	_bdef.angularDamping = value;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::awake(bool value)
{
	_bdef.awake = value;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::linearDamping(float value)
{
	// TODO: insert return statement here
	_bdef.linearDamping = value;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::rotation(bool isFixed)
{
	_bdef.fixedRotation = isFixed;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::angularVelocity(float velocity)
{
	// TODO: insert return statement here
	_bdef.angularVelocity = velocity;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::position(b2Vec2 position, float ppm)
{
	_bdef.position.Set(position.x / ppm, position.y / ppm);
	// // O: insert return statement here
	return *this;
}

b2BodyBuilder& b2BodyBuilder::position(float x, float y, float ppm)
{
	// // O: insert return statement here
	_bdef.position.Set(x / ppm, y / ppm);
	// // O: insert return statement here
	return *this;
}

b2BodyBuilder& b2BodyBuilder::newRectangleFixture(float hx, float hy, float ppm)
{
	_shape = std::make_unique<b2PolygonShape>();
	_fdef.shape = _shape.get();

	auto* shape = dynamic_cast<b2PolygonShape*>(_shape.get());
	shape->SetAsBox(hx / ppm, hy / ppm);
	return *this;
}

b2BodyBuilder& b2BodyBuilder::newRectangleFixture(float hx, float hy, const b2Vec2& center, float angle, float ppm)
{
	// TODO: insert return statement here
	_shape = std::make_unique<b2PolygonShape>();
	_fdef.shape = _shape.get();

	auto* shape = dynamic_cast<b2PolygonShape*>(_shape.get());
	shape->SetAsBox(hx / ppm, hy / ppm, { center.x / ppm, center.y / ppm }, angle);
	return *this;
}

b2BodyBuilder& b2BodyBuilder::newPolygonFixture(const b2Vec2* vertices, size_t count, float ppm)
{
	_shape = std::make_unique<b2PolygonShape>();
	_fdef.shape = _shape.get();

	auto* shape = dynamic_cast<b2PolygonShape*>(_shape.get());
	auto* scaledVertices = new b2Vec2[count];
	//b2Vec2 scaledVertices[count];
	for (size_t i = 0; i < count; i++) {
		scaledVertices[i] = { vertices[i].x / ppm, vertices[i].y / ppm };
	}
	shape->Set(scaledVertices, count);
	delete[] scaledVertices;
	// // O: insert return statement here
	return *this;
}

b2BodyBuilder& b2BodyBuilder::newPolylineFixture(const b2Vec2* vertices, size_t count, float ppm)
{
	// // O: insert return statement here
	_shape = std::make_unique<b2ChainShape>();
	_fdef.shape = _shape.get();

	auto* shape = dynamic_cast<b2ChainShape*>(_shape.get());
	//b2Vec2 scaledVertices[count];
	auto* scaledVertices = new b2Vec2[count];
	for (size_t i = 0; i < count; i++) {
		scaledVertices[i] = { vertices[i].x / ppm, vertices[i].y / ppm };
	}
	shape->CreateChain(scaledVertices, count, b2Vec2_zero, b2Vec2_zero);
	delete[] scaledVertices;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::newEdgeShapeFixture(const b2Vec2& vertex1, const b2Vec2& vertex2, float ppm)
{
	// // O: insert return statement here
	_shape = std::make_unique<b2EdgeShape>();
	_fdef.shape = _shape.get();

	auto* shape = dynamic_cast<b2EdgeShape*>(_shape.get());
	b2Vec2 scaledVertex1 = { vertex1.x / ppm, vertex1.y / ppm };
	b2Vec2 scaledVertex2 = { vertex2.x / ppm, vertex2.y / ppm };
	shape->SetTwoSided(scaledVertex1, scaledVertex2);
	return *this;
}

b2BodyBuilder& b2BodyBuilder::newCircleFixture(float radius, float ppm)
{
	// // O: insert return statement here
	_shape = std::make_unique<b2CircleShape>();
	_fdef.shape = _shape.get();

	auto* shape = dynamic_cast<b2CircleShape*>(_shape.get());
	//shape->m_p.Set(centerPos.x / ppm, centerPos.y / ppm);
	shape->m_radius = radius / ppm;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::newCircleFixture(float radius, float scale, float ppm)
{
	// // O: insert return statement here
	_shape = std::make_unique<b2CircleShape>();
	_fdef.shape = _shape.get();

	auto* shape = dynamic_cast<b2CircleShape*>(_shape.get());

	// Calculate the new radius based on the scale factor
	float currentRadius = radius / ppm;
	float newRadius = currentRadius * scale;

	// Set the new radius
	shape->m_radius = newRadius;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::newCircleFixture(const b2Vec2& centerPos, float radius, float ppm)
{
	// // O: insert return statement here
	_shape = std::make_unique<b2CircleShape>();
	_fdef.shape = _shape.get();

	auto* shape = dynamic_cast<b2CircleShape*>(_shape.get());
	shape->m_p.Set(centerPos.x / ppm, centerPos.y / ppm);
	shape->m_radius = radius / ppm;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::categoryBits(short categoryBits)
{
	_fdef.filter.categoryBits = categoryBits;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::maskBits(short maskBits)
{
	_fdef.filter.maskBits = maskBits;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::setSensor(bool isSensor)
{
	// // O: insert return statement here
	_fdef.isSensor = isSensor;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::friction(float friction)
{
	_fdef.friction = friction;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::groupIndex(int index)
{
	_fdef.filter.groupIndex = index;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::denstity(float denstity)
{
	// TODO: insert return statement here
	_fdef.density = denstity;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::restitution(float restitution)
{
	// // O: insert return statement here
	_fdef.restitution = restitution;
	return *this;
}

b2BodyBuilder& b2BodyBuilder::setUserData(void* userData)
{
	_userData = userData;
	return *this;
}

b2Fixture* b2BodyBuilder::buildFixture()
{
	_fixture = _body->CreateFixture(&_fdef);
	_fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(_userData);
	_fdef = b2FixtureDef(); // Clear _fdef data
	return _fixture;
}

b2Body* b2BodyBuilder::buildBody()
{
	_body = _world->CreateBody(&_bdef);
	return _body;
}