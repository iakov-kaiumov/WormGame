#ifndef LIQUIDFUN_MODEL_H
#define LIQUIDFUN_MODEL_H


#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2World.h>
#include "../view/DebugDraw.h"

class Model {
    public:
        Model(b2Vec2 gravity);
        void createGroundBox(float width, float height);
        b2Body *createSquare(b2Vec2 pos);
        b2Body *createCircle(b2Vec2 pos);
        b2ParticleSystem *createElasticBody(b2Vec2 pos);
        b2ParticleSystem *createOcean(b2Vec2 pos);
        void setDebugDraw(DebugDraw *draw);
        void step();
        void addBody(b2Body *body);
        void destroyBody(b2Body *body);
        b2World world;
        b2MouseJoint *mouseJoint = nullptr;
        b2Body *ground; // Mouse joint anchor
        std::vector<b2Body *> bodies;
    private:
        b2ParticleSystemDef particleSystemDef;
        /* constants for time step and physics accuracy */
        const float timeStep = 1.0f / 60.0f;
        const int velocityIterations = 6;
        const int positionIterations = 2;
};


#endif //LIQUIDFUN_MODEL_H
