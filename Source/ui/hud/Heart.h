#pragma once

#include "axmol.h"

class Heart : public ax::Sprite
{
public:
	Heart();
	~Heart() = default;

	static Heart* create();

	bool init() override;

	void setFilled(bool filled);
};