#pragma once

#include "axmol.h"

class Coin : public ax::Sprite
{
public:
    Coin();
    ~Coin() override = default;

    static Coin* create();

    bool init() override;
    inline int getValue() const { return m_value; }

private:
    int m_value{5};
};
