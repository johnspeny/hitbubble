#pragma once

#include "axmol.h"

/*
	If you want particles to have an initial rotation when emitted,
	you might use setStartSpin. If you want particles to continuously rotate as they move,
	you might use setTangentialAccel.
*/

class FireWorksParticle : public ax::ParticleSystemQuad {
public:
	static FireWorksParticle* create();
	void setupParticleProperties(ax::Node* node, const ax::Vec2& position, const std::string& texturePath);
};