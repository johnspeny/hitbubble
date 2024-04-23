#include "DustParticle.h"
#include "scene/GameScene.h"
#include "scene/SceneManager.h"

DustParticle* DustParticle::create()
{
	auto pRet = new DustParticle();
	//numberOfParticles
	if (pRet->initWithTotalParticles(50)) {
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

void DustParticle::setupParticleProperties(const ax::Vec2& position, const std::string& texturePath)
{
	auto scene = SceneManager::getInstance().getCurrentScene<GameScene>();

	setTexture(Director::getInstance()->getTextureCache()->addImage(texturePath));

	// duration | how long the particle will continue to be emmitted
	// -1 means it only dies when manually removed
	// duration is the lifetime of the particle
	setDuration(-1);

	// emission mode
	// setEmitterMode(ParticleSystem::Mode::GRAVITY);

	/***************** Gravity Mode ********************************************************/
	// gravity | force pulling the particles in x, y directions
	setGravity({ 0, 100 });

	// speed of particles | how fast the particles move
	setSpeed(10);// base speed is 60
	// initial speed randomness of the emitted particle
	// this introduces randomness to the initial speed of each particle,
	// so actual speeds of each particle will be in the range 60 - 20 to 60 + 20
	setSpeedVar(10); // base is 0 units

	// TangentialAcceleration affects the rotational speed of particles as they move,
	// providing a tangential force that causes them to rotate
	// positive value for clockwise and negative for counter clockwise
	setTangentialAccel(80);
	// Adds randomness to the intial rate of the the tangentialAcc of each particle
	setTangentialAccelVar(0);

	// defines how particles move towards or away from the center of the particles
	// or where the particles are supposed to emerge from
	// a positive value means an outward or outburst
	// a negative value mean inwards movement
	setRadialAccel(70);
	// Adds randomness to the initial radial acceleration
	// The actual radial acceleration of each particle will be randomly adjusted within
	// the range of 10 - 20 to 10 + 20. As a result, particles will have different
	// radial accelerations, adding variety to their motion.
	setRadialAccelVar(10);

	/***************** Particle Configuration *******************************************/
	// the direction the particle will be emitted eg 90 upwards
	setAngle(0);
	// Adds randomness to the initial angle of each particle
	// The actual angle of which each particle will be emitted in at the range of
	// 90 + 45 to 90 - 45. As a result, each particle will have different angle of emission
	setAngleVar(360);

	//foodparticles->setEndSize(ParticleSystem::START_SIZE_EQUAL_TO_END_SIZE);
	// sets the initial size of the particle
	setStartSize(5);
	// Adds randomness to the inital size of each particle
	// the actual start size of each particle emitted will be at a range of
	// 5 + 2.0f to 5 - 2.0f.
	setStartSizeVar(5);
	// as particles age and reach their lifespan, they will have a size of X
	setEndSize(2);
	setEndSizeVar(1);

	// All particles will live for the same amount of time before being removed
	setLife(1.0f);
	//The actual lifespan of each particle will be randomly adjusted within the range of 0.30 - 0.20 to 0.30 + 0.20.
	//As a result, particles will have different lifespans, adding variety to their existence.
	setLifeVar(0.30f);

	// spin of particles
	// start spin affects the initial rotational speed of particles when they are emitted
	//setStartSpin(60);
	// As particles age and reach the end of their lifespan, they will have a rotation or spin.
	//setEndSpin(0);
	//setEndSpinVar(0);

	// spawn
	//setSpawnAngleVar(180);

	// color of particles
	Color4F startColor(0.76f, 0.25f, 0.12f, 1.0f);
	setStartColor(startColor);

	Color4F startColorVar(0.f, 0.f, 0.f, 0.0f);
	setStartColorVar(startColorVar);

	Color4F endColor(0.f, 0.f, 0.f, 1.0f);
	setEndColor(endColor);

	Color4F endColorVar(0.f, 0.f, 0.f, 0.f);
	setEndColorVar(endColorVar);

	// emits per second
	setEmissionRate(getTotalParticles() / getLife());

	// additive
	setBlendAdditive(true);

	// emitter position
	setPosition(position);
	//setPosVar({ 10, 10 });
	setPosVar({ 10, 0 });

	// add to scene
	scene->addChild(this, 10);

	// Run the particle system and remove it after a certain duration
	runAction(Sequence::create(
		DelayTime::create(0.5f),
		RemoveSelf::create(),
		nullptr));
}