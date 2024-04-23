#include "b2DebugRenderer.h"

b2DebugRenderer::b2DebugRenderer(b2World* world) :
	_world(world),
	mB2DebugDraw(std::make_unique<GLESDebugDraw>(ax::Director::getInstance()->getVisibleSize().height / 10))
{
}

b2DebugRenderer* b2DebugRenderer::create(b2World* world)
{
	auto pRet = new b2DebugRenderer(world);
	if (pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		AX_SAFE_DELETE(pRet);
	}
	return pRet;
}

void b2DebugRenderer::draw(Renderer* renderer, const Mat4& transform, uint32_t flags)
{
	Node::draw(renderer, transform, flags);

	auto director = Director::getInstance();
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	mB2DebugDraw->GetDrawNode()->clear();
	_world->DebugDraw();
	mB2DebugDraw->GetDrawNode()->draw(renderer, transform, flags);
	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

bool b2DebugRenderer::init()
{
	//mB2DebugDraw = new GLESDebugDraw(100);
	_world->SetDebugDraw(mB2DebugDraw.get());
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	//flags += b2Draw::e_aabbBit;
	//flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	mB2DebugDraw->SetFlags(flags);
	return true;
}