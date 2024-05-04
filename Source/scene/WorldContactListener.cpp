#include "WorldContactListener.h"
#include "Constants.h"
#include "CollisionEvent.h"
#include "GameUtils.h"
#include "actor/Ball.h"
#include "entity/Wall.h"
#include "helpers/Logger.h"
#include "scene/GameScene.h"
#include "scene/SceneManager.h"
#include "MyEnums.h"
#include "actor/item/Fruit.h"

using namespace category_bits;

WorldContactListener::WorldContactListener()
{
    GameUtils::PPM::initVars();
}

void WorldContactListener::BeginContact(b2Contact* contact)
{
    auto snmg = SceneManager::getInstance().getCurrentScene<GameScene>();
    if (!snmg->_isGameOverHandled && !snmg->_isWinGameHandled && !snmg->_isReviveGame)
    {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        int cDef = fixtureA->GetFilterData().categoryBits | fixtureB->GetFilterData().categoryBits;

        switch (cDef)
        {
        case category_bits::kWall | category_bits::kBall:
        {
            b2Fixture* ballFixture = GetTargetFixture(category_bits::kBall, fixtureA, fixtureB);
            b2Fixture* wallFixture = GetTargetFixture(category_bits::kWall, fixtureA, fixtureB);

            if (ballFixture && wallFixture)
            {
                auto ball = reinterpret_cast<Ball*>(ballFixture->GetUserData().pointer);

                if (wallFixture->GetUserData().pointer ==
                    reinterpret_cast<uintptr_t>(reinterpret_cast<void*>(WallFixtureType::GROUND)))
                {
                    // disable player from colliding with ball after hitting wall
                    b2Filter filterData;
                    filterData.categoryBits = category_bits::kBall;  // Category to which the fixture belongs
                    filterData.maskBits =
                        kHead | kHair | kWall | kWallRight | kWallLeft;  // Categories it can collide with
                    filterData.groupIndex = 0;                           // Group index (optional)
                    ballFixture->SetFilterData(filterData);

                    // game over
                    auto snmg = SceneManager::getInstance().getCurrentScene<GameScene>();
                    if (!snmg->_isWinGameHandled && !snmg->_isReviveGame)
                    {
                        ball->setGameOver(true);
                    }
                }
            }
        }
        break;

        case category_bits::kHair | category_bits::kBall:
        {
            b2Fixture* ballFixture = GetTargetFixture(category_bits::kBall, fixtureA, fixtureB);
            b2Fixture* hairFixture = GetTargetFixture(category_bits::kHair, fixtureA, fixtureB);

            // CLOG("at hit");

            if (ballFixture && hairFixture)
            {
                auto ball = reinterpret_cast<Ball*>(ballFixture->GetUserData().pointer);

                if (!ball->getIsHitHair())
                {
                    ball->setIsHitHair(true);
                    // CLOG("hit");
                }
            }
        }
        break;

        case category_bits::kItem | category_bits::kBall:
        {
            b2Fixture* ballFixture = GetTargetFixture(category_bits::kBall, fixtureA, fixtureB);
            b2Fixture* itemFixture = GetTargetFixture(category_bits::kItem, fixtureA, fixtureB);

            if (ballFixture && itemFixture)
            {
                auto item = reinterpret_cast<Fruit*>(itemFixture->GetUserData().pointer);
                // CLOG("ball hit item");
                item->startContact();
                auto snmg = SceneManager::getInstance().getCurrentScene<GameScene>();
                snmg->handleContactItemBall(item);
            }
        }
        break;

        case category_bits::kItem | category_bits::kWall:
        {
            b2Fixture* wallFixture = GetTargetFixture(category_bits::kWall, fixtureA, fixtureB);
            b2Fixture* itemFixture = GetTargetFixture(category_bits::kItem, fixtureA, fixtureB);

            if (wallFixture && itemFixture)
            {
                auto item = reinterpret_cast<Fruit*>(itemFixture->GetUserData().pointer);

                if (wallFixture->GetUserData().pointer ==
                    reinterpret_cast<uintptr_t>(reinterpret_cast<void*>(WallFixtureType::GROUND)))
                {
                    if (!item->_isHitWallHandled)
                    {
                        item->setIsHitWall(true);
                        item->startWallContact();
                    }

                    // Recalculate mass properties if needed
                    // marioBody->ResetMassData();
                }
            }
        }
        break;

        default:
            break;
        }
    }
}

void WorldContactListener::EndContact(b2Contact* contact)
{
    auto snmg = SceneManager::getInstance().getCurrentScene<GameScene>();
    if (!snmg->_isGameOverHandled && !snmg->_isWinGameHandled && !snmg->_isReviveGame)
    {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        int cDef = fixtureA->GetFilterData().categoryBits | fixtureB->GetFilterData().categoryBits;
        switch (cDef)
        {
        case category_bits::kItem | category_bits::kBall:
        {
            b2Fixture* ballFixture = GetTargetFixture(category_bits::kBall, fixtureA, fixtureB);
            b2Fixture* itemFixture = GetTargetFixture(category_bits::kItem, fixtureA, fixtureB);

            if (ballFixture && itemFixture)
            {
                auto item = reinterpret_cast<Fruit*>(itemFixture->GetUserData().pointer);

                if (item->getNumContacts() == item->getPoints())
                {
                    // disable player from colliding with ball after hitting wall
                    b2Filter filterData;
                    filterData.categoryBits = category_bits::kItem;            // Category to which the fixture belongs
                    filterData.maskBits     = kWall | kWallRight | kWallLeft;  // Categories it can collide with
                    filterData.groupIndex   = 0;                               // Group index (optional)
                    itemFixture->SetFilterData(filterData);
                }
            }
        }
        }
    }
}

void WorldContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {}

void WorldContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {}

b2Fixture* WorldContactListener::GetTargetFixture(short targetCategoryBits, b2Fixture* f1, b2Fixture* f2) const
{
    b2Fixture* targetFixture = nullptr;

    if (f1->GetFilterData().categoryBits == targetCategoryBits)
    {
        targetFixture = f1;
    }
    else if (f2->GetFilterData().categoryBits == targetCategoryBits)
    {
        targetFixture = f2;
    }
    return targetFixture;
}
