#ifndef _QUERYCALLBACK_H_
#define _QUERYCALLBACK_H_

#include "box2d/box2d.h"

class QueryCallback : public b2QueryCallback
{
public:
	QueryCallback(const b2Vec2& point);
	bool ReportFixture(b2Fixture* fixture);
	b2Vec2 m_point;
	b2Fixture* m_fixture;
};

#endif // !_QUERYCALLBACK_H_
