#ifndef LIQUIDFUN_BODYDATA_H
#define LIQUIDFUN_BODYDATA_H

#include <SFML/Graphics/Color.hpp>
#include <vector>

struct BodyTextureData {
    sf::Texture *texture;
    sf::Sprite *sprite;
    sf::Color fillColor;
    sf::Color lineColor;
};

#endif //LIQUIDFUN_BODYDATA_H
