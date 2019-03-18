#ifndef LIQUIDFUN_DRAW_H
#define LIQUIDFUN_DRAW_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <Box2D/Box2D.h>
#include <SFML/Graphics/Text.hpp>
#include <sstream>
#include "BasicDraw.h"

class Draw : public BasicDraw {
public:
    void init(sf::RenderWindow *window) override;
    void step(b2World *world, GameInfo *gameInfo) override;
private:
    //// Draw any type of shape
    void DrawShape(b2Fixture* fixture, const b2Transform& xf, const sf::Color& fillColor, const sf::Color& lineColor);

    //// Draw joints
    void DrawJoint(b2Joint* joint);

    /// Draw a solid closed polygon provided in CCW order.
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const sf::Color& fillColor, const sf::Color& lineColor);

    /// Draw a circle.
    void DrawCircle(const b2Vec2& center, float32 radius, const sf::Color& fillColor, const sf::Color& lineColor);

    /// Draw a solid circle.
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis,
            const sf::Color& fillColor, const sf::Color& lineColor);

    /// Draw a line segment.
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const sf::Color& lineColor);

    /// Draw a transform. Choose your own length scale.
    void DrawTransform(const b2Transform& xf);

    //// Draw particles
    void DrawParticles(int type, const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count);

    const sf::Color BACKGROUND_COLOR = sf::Color(14, 43, 89);
    sf::RenderWindow *m_window;
    std::stringstream stream;
    sf::Text fpsCounter;
    sf::Text helpText;
    sf::Font mainFont;
};


#endif //LIQUIDFUN_DRAW_H
