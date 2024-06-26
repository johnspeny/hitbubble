#pragma once

#include "Item.h"

class GameScene;

class Fruit : public Item
{
private:
    float m_scale;
    float m_radius;
    GameScene* m_scene;
    void defineBody(float x, float y);

    ax::Label* lifeLabel;
    int m_lifeVal;
    bool m_isHitWall{false};

public:
    bool _isHitWallHandled{false};
    explicit Fruit(int lifeVal);
    virtual ~Fruit() = default;

    bool showOnGameScene(const Vec2& position) override;

    void update(float dt) override;

    const std::string& selectRandomSpriteName(const std::vector<std::string>& spriteNames) const override;

    inline std::string getCategory() const override { return "Fruit"; }

    inline std::vector<std::string> getSpriteNames() const override
    {
        return {"CloseNormal.png", "CloseNormal.png", "CloseNormal.png", "CloseNormal.png",
                "CloseNormal.png", "CloseNormal.png", "CloseNormal.png"};
    }

    virtual void startContact() override;

    inline float getRadius() const { return m_radius; };

    // hit wall
    void setIsHitWall(bool ishitWall);

    inline bool getIsHitWall() const { return m_isHitWall; }
};
