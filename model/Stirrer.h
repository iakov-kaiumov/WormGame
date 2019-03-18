#ifndef LIQUIDFUN_STIRRER_H
#define LIQUIDFUN_STIRRER_H

#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Box2D.h>

class Stirrer {
public:
    Stirrer(b2World *world, b2Vec2 pos, float width, float height) {
        b2BodyDef anchorDef;
        anchorDef.position = pos;
        b2Body *anchor = world->CreateBody(&anchorDef);

        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.allowSleep = false;
        bd.position = pos;
        b2Body* body = world->CreateBody(&bd);

        b2PolygonShape shape;
        shape.SetAsBox(width / 2, height / 2, b2Vec2(0.0f, 0.0f), 0.0);
        body->CreateFixture(&shape, 5.0f);

        b2PrismaticJointDef prismaticJointDef;
        prismaticJointDef.bodyA = anchor;
        prismaticJointDef.bodyB = body;
        prismaticJointDef.collideConnected = true;
        prismaticJointDef.localAxisA.Set(1,0);
        prismaticJointDef.localAnchorA = body->GetPosition();
        prismaticJointDef.enableMotor = true;
        prismaticJointDef.maxMotorForce = 10000;
        joint = (b2PrismaticJoint *) world->CreateJoint(&prismaticJointDef);
    }

    void move() {
        time += 0.01f;
        joint->SetMotorSpeed(amp * sin(freq * time));
    }

    float32 amp = 1.2f;
    float32 freq = 1.f;
private:
    float time = 0;
    b2PrismaticJoint *joint;
};
#endif //LIQUIDFUN_STIRRER_H
