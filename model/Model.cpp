#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Draw.h>
#include <iostream>
#include "Model.h"

Model::Model(b2Vec2 gravity) : world(gravity) {
    world.SetAllowSleeping(true);
    // Create mouse joint
    // This is not the body of the bounding box
    // This body exists to serve as an anchor point for the mouse joint
    b2BodyDef bodyDef;
    ground = world.CreateBody(&bodyDef);
}

void Model::setDebugDraw(DebugDraw *draw) {
    world.SetDebugDraw(draw);
}

void Model::step() {
    world.Step(timeStep, velocityIterations, positionIterations);
}

void Model::addBody(b2Body *body) {
    bodies.push_back(body);
}

void Model::destroyBody(b2Body *body) {
    world.DestroyBody(body);
    bodies.erase(std::remove(bodies.begin(), bodies.end(), body), bodies.end());
}

void Model::createGroundBox(float width, float height) {
    b2BodyDef boundingBoxDef;
    boundingBoxDef.type = b2_staticBody;
    float xPos = width / 2.f;
    float yPos = 0.5f;
    boundingBoxDef.position.Set(xPos, yPos);

    b2Body* boundingBoxBody = world.CreateBody(&boundingBoxDef);

    b2PolygonShape boxShape;
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.f;
    fixtureDef.shape = &boxShape;
    boxShape.SetAsBox(width, 0.5f, b2Vec2(0.f, 0.f), 0.f);
    boundingBoxBody->CreateFixture(&fixtureDef); //Top

    yPos = height - 1.f;
    boxShape.SetAsBox(width, 0.5f, b2Vec2(0.f, yPos), 0.f);
    boundingBoxBody->CreateFixture(&fixtureDef); //Bottom

    yPos = height - 1.f;
    boxShape.SetAsBox(width, 0.5f, b2Vec2(10.f, yPos), b2_pi / 20);
    boundingBoxBody->CreateFixture(&fixtureDef); // Angled bottom

    xPos -= 0.5f;
    boxShape.SetAsBox(0.5f, height, b2Vec2(-xPos, 0.f), 0.f);
    boundingBoxBody->CreateFixture(&fixtureDef);//Left

    boxShape.SetAsBox(0.5f, height, b2Vec2(xPos, 0.f), 0.f);
    boundingBoxBody->CreateFixture(&fixtureDef);//Right

    b2Vec2 pos(width / 2.f, height / 2.f);
}

b2Body *Model::createSquare(b2Vec2 pos) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = pos;
    b2Body *body = world.CreateBody(&bodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox(1.f, 1.f, b2Vec2(0.f, 0.f), 0);
    body->CreateFixture(&boxShape, 1.f);

    return body;
}

b2Body *Model::createCircle(b2Vec2 pos) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = pos;
    b2Body *body = world.CreateBody(&bodyDef);


    b2CircleShape circleShape;
    circleShape.m_radius = 0.5f;
    circleShape.m_p.SetZero();

    body->CreateFixture(&circleShape, 0.5f);

    return body;
}

b2ParticleSystem *Model::createElasticBody(b2Vec2 pos) {

    b2ParticleSystem* m_particleSystem = world.CreateParticleSystem(&particleSystemDef);
    m_particleSystem->SetRadius(0.20f);
    m_particleSystem->SetParticleFlags(0, 1);
    {
        b2PolygonShape shape;
        shape.SetAsBox(5.0f, 0.6f);
        b2ParticleGroupDef pd;
        pd.strength = 1.f;
        pd.flags = b2_elasticParticle;
        pd.groupFlags = b2_solidParticleGroup;
        pd.position = pos;
        pd.angle = 0.0f;
        pd.angularVelocity = 1.0f;
        pd.shape = &shape;
        pd.color.Set(0, 250, 100, 255);
        m_particleSystem->CreateParticleGroup(pd);
    }
    return m_particleSystem;
}

b2ParticleSystem *Model::createOcean(b2Vec2 pos) {

    b2ParticleSystem* m_particleSystem = world.CreateParticleSystem(&particleSystemDef);
    m_particleSystem->SetRadius(0.15f);
    m_particleSystem->SetDamping(0.2f);
    m_particleSystem->SetDensity(1.f);
    m_particleSystem->SetParticleFlags(0, 0);
    {
        b2ParticleGroupDef pd;
        pd.position = pos;
        pd.flags = b2_waterParticle;
        pd.color.Set(200, 255, 255, 255);
        b2PolygonShape shape;
        shape.SetAsBox(16.f, 5.f, b2Vec2(0.0f, 0.0f), 0.0);

        pd.shape = &shape;
        m_particleSystem->CreateParticleGroup(pd);
    }
    return m_particleSystem;
}
