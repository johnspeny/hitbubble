#include "Coin.h"

Coin::Coin() : m_value{5} {}

Coin* Coin::create()
{
    auto pRet = new Coin();
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

bool Coin::init()
{
    initWithFile("ball_volley2.png");

    return true;
}
