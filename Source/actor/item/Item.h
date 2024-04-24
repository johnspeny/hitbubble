#pragma once

#include "DynamicEntity.h"

class Item : public DynamicEntity
{
public:
    Item();
    virtual ~Item()                                                                                      = default;
    virtual const std::string& selectRandomSpriteName(const std::vector<std::string>& spriteNames) const = 0;
    virtual std::string getCategory() const                                                              = 0;
    virtual std::vector<std::string> getSpriteNames() const                                              = 0;
    virtual bool showOnGameScene(const Vec2& position) override                                          = 0;
    virtual void update(float dt) override;

    // store original position of this item to be placed on the board
    void originalPosition(float itemX, float itemY);
    inline b2Vec2 getOriginalPosition() { return b2Vec2(m_itemX, m_itemY); }

    virtual void startContact();
    virtual void startWallContact();
    virtual void endContact();
    virtual int getNumContacts() const;
    virtual int getNumWallContacts() const;

protected:
    float m_itemX;
    float m_itemY;
    int m_numContacts;
    int m_numWallContacts;
};
