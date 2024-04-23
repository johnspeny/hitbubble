#ifndef __GLESDEBUGDRAW_H__
#define __GLESDEBUGDRAW_H__

#include "cocos2d.h"
#include "box2d/box2d.h"

using namespace ax;

class GLESDebugDraw : public b2Draw
{
public:
	GLESDebugDraw();
	explicit GLESDebugDraw(float ratio);
	virtual ~GLESDebugDraw();

	void DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color) override;
	void DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color) override;
	void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;
	void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
	void DrawTransform(const b2Transform& xf) override;
	void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;

	void setRatio(float ratio) { _ratio = ratio; }

	DrawNode* GetDrawNode() const { return _drawNode; }

protected:
	float _ratio;

	DrawNode* _drawNode = nullptr;
};

#endif // !__GLESDEBUGDRAW_H__
