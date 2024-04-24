#include "Item.h"
#include "GameUtils.h"
#include "helpers/Logger.h"

Item::Item() : m_itemX{}, m_itemY{}, m_numContacts{0}, m_numWallContacts{0} {}

void Item::update(float dt)
{
    if (!m_body)
    {
        return;
    }

    DynamicEntity::update(dt);
}

void Item::originalPosition(float itemX, float itemY)
{
    m_itemX = itemX;
    m_itemY = itemY;
}

void Item::startContact()
{
    m_numContacts++;
}

void Item::startWallContact()
{
    m_numWallContacts++;
}

void Item::endContact()
{
    m_numContacts--;
}

int Item::getNumContacts() const
{
    return m_numContacts;
}

int Item::getNumWallContacts() const
{
    return m_numWallContacts;
}
