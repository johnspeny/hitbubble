#include "Heart.h"
#include "helpers/Logger.h"

Heart::Heart()
{
}

Heart* Heart::create()
{
	auto pRet = new Heart();
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

bool Heart::init()
{
	initWithFile("h.png");

	//setFilled(true);

	return true;
}

void Heart::setFilled(bool filled)
{
	setColor(filled ? ax::Color3B::RED : ax::Color3B::GRAY);
}