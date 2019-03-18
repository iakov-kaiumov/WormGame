#ifndef LIQUIDFUN_BOAT_H
#define LIQUIDFUN_BOAT_H


#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <fstream>
#include "Vehicle.h"
#include "iostream"
#include "algorithm"
#include "BodyTextureData.h"
#include "../utils/Utils.h"

class Boat : public Vehicle {
public:
    Boat(b2World *world, b2Vec2 pos, const char *fixturePath, const char *spritePath,
            float scale = 1.f, float density = 0.5f) {
        this->scale = scale;
        this->fixturePath = const_cast<char*>(fixturePath);

        gravity = world->GetGravity();
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = pos;
        body = world->CreateBody(&bodyDef);
        loadFixturesFromFile(fixturePath, scale, density);
        // Load and set texture
        auto *data = new BodyTextureData();
        data->texture = new sf::Texture();
        data->texture->loadFromFile(spritePath);
        data->texture->setSmooth(true);
        data->sprite = new sf::Sprite(*data->texture);
        data->sprite->scale(scale, scale);
        float x = data->sprite->getTexture()->getSize().x;
        float y = data->sprite->getTexture()->getSize().y;
        data->sprite->setOrigin(x / 2, y / 2);
        body->SetUserData(data);
    }

    void accelerate(float power) override {
        float32 angle = body->GetAngle();
        b2Vec2 vec(power * cos(angle), power * sin(angle));
        body->ApplyForce(vec * gravity.Length() * body->GetMass(), body->GetPosition(), true);
    }

    bool checkRotation() {
        float32 angle = body->GetAngle();
        if (abs(angle) > b2_pi / 2 && body->GetFixtureList()->GetDensity() < 1.f) {
            b2Fixture *f;
            while ((f = body->GetFixtureList()) != nullptr) {
                body->DestroyFixture(f);
            }
            loadFixturesFromFile(fixturePath, scale, 6.f);
        }
        return !body->IsAwake();
    }
private:
    b2Vec2 gravity;
    float scale;
    char *fixturePath;
};

#endif //LIQUIDFUN_BOAT_H
