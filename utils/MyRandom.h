#ifndef LIQUIDFUN_MYRANDOM_H
#define LIQUIDFUN_MYRANDOM_H

#include <random>
#include <ctime>

class MyRandom {
public:
    float rRandom(float left, float right) {
        std::uniform_real_distribution<float> urd(left, right);
        return urd(rd);
    }

    unsigned int iRandom(unsigned int left, unsigned int right) {
        std::uniform_int_distribution<unsigned int> urd(left, right);
        return urd(rd);
    }
private:
    std::random_device rd;
};

#endif //LIQUIDFUN_MYRANDOM_H
