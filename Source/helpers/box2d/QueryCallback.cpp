#include "QueryCallback.h"

QueryCallback::QueryCallback(const b2Vec2& point)
{
	m_point = point;
	m_fixture = nullptr;
}

bool QueryCallback::ReportFixture(b2Fixture* fixture)
{
	b2Body* body = fixture->GetBody();
	if (body->GetType() == b2_dynamicBody)
	{
		bool inside = fixture->TestPoint(m_point);
		if (inside)
		{
			m_fixture = fixture;
			return false;
		}
	}
	return true;
}