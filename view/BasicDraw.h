#ifndef LIQUIDFUN_BASICDRAW_H
#define LIQUIDFUN_BASICDRAW_H

#include <Box2D/Dynamics/b2World.h>
#include "../controller/GameInfo.h"

class BasicDraw {
public:
    virtual void init(sf::RenderWindow *window) = 0;
    virtual void step(b2World *world, GameInfo *gameInfo) = 0;
};

#endif //LIQUIDFUN_BASICDRAW_H
