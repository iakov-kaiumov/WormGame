#ifndef LIQUIDFUN_VEHICLE_H
#define LIQUIDFUN_VEHICLE_H


#include <Box2D/Common/b2Math.h>
#include "Body.h"

class Vehicle : public Body {
public:
    virtual void accelerate(float power) = 0;
};


#endif //LIQUIDFUN_VEHICLE_H
