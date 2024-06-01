#pragma once

#include "actor/character/Head.h"
#include "actor/character/Torso.h"
#include "actor/character/Hair.h"

class GameScene;

class Character
{
public:
    Character();
    virtual ~Character() = default;

    // parts
    std::unique_ptr<Head> m_head{};
    std::unique_ptr<Hair> m_hair{};
    std::unique_ptr<Torso> m_torso{};

    // joints
    b2Joint* hair_head_joint{};
    b2Joint* head_torso_joint{};

    // create
    void createHumanBody(float x, float y);

    // character update parts
    void update(float dt) const;

    ax::Vec2 originPos;  // useful when resetting the position of character

private:
    GameScene* _gameScene;
    void connectBodyPartsWithJoints();
};
