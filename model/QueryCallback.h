//
// Created by yasha on 23.02.19.
//

#ifndef LIQUIDFUN_QUERYCALLBACK_H
#define LIQUIDFUN_QUERYCALLBACK_H


#include <Box2D/Box2D.h>

class QueryCallback : public b2QueryCallback {
    public:
        QueryCallback(const b2Vec2& point);
        bool ReportFixture(b2Fixture* fixture) override;
        b2Vec2 m_point;
        b2Fixture* m_fixture;
};


#endif //LIQUIDFUN_QUERYCALLBACK_H
