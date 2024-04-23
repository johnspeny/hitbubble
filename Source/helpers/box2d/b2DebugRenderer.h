#ifndef __B2DEBUGRENDERER_H__
#define __B2DEBUGRENDERER_H__

#include "cocos2d.h"
#include "box2d/box2d.h"
#include "GLESDebugDraw.h"

using namespace ax;

class b2DebugRenderer : public ax::Node
{
public:
	explicit b2DebugRenderer(b2World* world);
	virtual ~b2DebugRenderer() = default;
	static b2DebugRenderer* create(b2World* world);
	virtual void draw(Renderer* renderer, const Mat4& transform, uint32_t flags) override;

	virtual bool init() override;
private:
	b2World* _world;
	std::unique_ptr<GLESDebugDraw> mB2DebugDraw;
};

#endif // !__B2DEBUGRENDERER_H__
