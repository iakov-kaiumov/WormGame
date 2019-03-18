#ifndef LIQUIDFUN_BODY_H
#define LIQUIDFUN_BODY_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Box2D/Dynamics/b2Body.h>

class Body {
public:
    void loadFixturesFromFile(const char *path, float scale = 1, float density = 0);
    b2Body *body;
};

#endif //LIQUIDFUN_BODY_H
