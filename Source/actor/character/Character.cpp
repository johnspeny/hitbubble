#include "Character.h"

#include "GameUtils.h"
#include "Constants.h"
#include "helpers/Logger.h"
#include "scene/GameScene.h"
#include "scene/SceneManager.h"
#include "helpers/box2d/b2BodyBuilder.h"

Character::Character()
{
    GameUtils::PPM::initVars();
    _gameScene = SceneManager::getInstance().getCurrentScene<GameScene>();
}

void Character::createHumanBody(float x, float y)
{
    // torso (main body)
    m_torso = std::make_unique<Torso>();
    m_torso->showOnGameScene({x, y});

    // head
    m_head = std::make_unique<Head>(m_torso.get());
    m_head->showOnGameScene({x, 0});

    // hair
    m_hair = std::make_unique<Hair>(m_head.get());
    m_hair->showOnGameScene({0, 0});

    // connection
    connectBodyPartsWithJoints();
}

void Character::update(float dt) const
{
    m_head->update(dt);

    // if head is hit by meteor
    // break the joint connecting the head n game over

    m_torso->update(dt);
    m_hair->update(dt);
}

void Character::connectBodyPartsWithJoints()
{
    auto snMgr = SceneManager::getInstance().getCurrentScene<GameScene>();
    auto world = snMgr->getWorld();

    // -------- join head to torso with revolute ---------------
    b2JointDef* jointDef;
    b2RevoluteJointDef revoluteJointDef;
    b2WeldJointDef weldJointDef;

    jointDef                        = &revoluteJointDef;
    revoluteJointDef.localAnchorA   = {0, -m_head->getRadius() / GameUtils::PPM::kPpm};
    revoluteJointDef.localAnchorB   = {0, m_torso->getHY() / GameUtils::PPM::kPpm};
    revoluteJointDef.referenceAngle = 0.0f;
    revoluteJointDef.enableLimit    = true;
    revoluteJointDef.lowerAngle     = -10.0f * M_PI / 180.0f;
    revoluteJointDef.upperAngle     = 10.0f * M_PI / 180.0f;
    revoluteJointDef.enableMotor    = false;

    revoluteJointDef.motorSpeed     = 0.0f;
    revoluteJointDef.maxMotorTorque = 0.0f;
    jointDef->bodyA                 = m_head->getBody();
    jointDef->bodyB                 = m_torso->getBody();
    jointDef->collideConnected      = false;
    head_torso_joint                = world->CreateJoint(jointDef);

    //// -------- join hair to head arm with a weldjoint ---------------
    jointDef                   = &weldJointDef;
    weldJointDef.localAnchorA  = {0, 0};
    weldJointDef.localAnchorB  = {0, m_head->getRadius() * 0.8f / GameUtils::PPM::kPpm};
    jointDef->bodyA            = m_hair->getBody();
    jointDef->bodyB            = m_head->getBody();
    jointDef->collideConnected = false;
    hair_head_joint            = world->CreateJoint(jointDef);
}
