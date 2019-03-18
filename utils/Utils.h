#ifndef LIQUIDFUN_UTILS_H
#define LIQUIDFUN_UTILS_H


#include <Box2D/Box2D.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

namespace sfdd {
    const float SCALE = 32.0f;
}

class Utils {
public:
    /// Convert Box2D's OpenGL style color definition[0-1] to SFML's color definition[0-255], with optional alpha byte[Default - opaque]
    static sf::Color b2ColorToSFML(const b2Color &color, sf::Uint8 alpha = 255) {
        return sf::Color(static_cast<sf::Uint8>(color.r * 255), static_cast<sf::Uint8>(color.g * 255),
                static_cast<sf::Uint8>(color.b * 255), alpha);
    }

    /// Convert Box2D's Particle OpenGL style color definition[0-1] to SFML's color definition[0-255]
    static sf::Color b2ParticleColorToSFML(const b2ParticleColor &color) {
        return sf::Color(color.r, color.g, color.b, color.a);
    }

    /// Convert Box2D's vector to SFML vector [Default - scales the vector up by SCALE constants amount]
    static sf::Vector2f B2VecToSFVec(const b2Vec2 &vector, bool scaleToPixels = true) {
        return sf::Vector2f(vector.x * (scaleToPixels ? sfdd::SCALE : 1.f), vector.y * (scaleToPixels ? sfdd::SCALE : 1.f));
    }

    //// Converts SFML's vector to Box2D's vector and downscales it so it fits Box2D's MKS units
    template<typename T>
    static b2Vec2 sfVecToB2Vec(sf::Vector2<T> vector) {
        return b2Vec2(vector.x / sfdd::SCALE, vector.y / sfdd::SCALE);
    }

    static float sfIntToB2Float(unsigned int x) {
        return x / sfdd::SCALE;
    }
};
#endif //LIQUIDFUN_UTILS_H
