#include <iostream>
#include "DebugDraw.h"
#include "../utils/Utils.h"

DebugDraw::DebugDraw(sf::RenderWindow &window) : m_window(&window) {}

void DebugDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
    sf::ConvexShape polygon(static_cast<size_t>(vertexCount));
    sf::Vector2f center;
    for (size_t i = 0; i < vertexCount; i++) {
        //polygon.setPoint(i, SFMLDraw::B2VecToSFVec(vertices[i]));
        sf::Vector2f transformedVec = Utils::B2VecToSFVec(vertices[i]);
        polygon.setPoint(i, sf::Vector2f(std::floor(transformedVec.x), std::floor(
                transformedVec.y))); // flooring the coords to fix distorted lines on flat surfaces
    }                                                                                                   // they still show up though.. but less frequently
    polygon.setOutlineThickness(-1.f);
    polygon.setFillColor(sf::Color::Transparent);
    polygon.setOutlineColor(Utils::b2ColorToSFML(color));
    m_window->draw(polygon);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
    sf::ConvexShape polygon(static_cast<size_t>(vertexCount));
    for (size_t i = 0; i < vertexCount; i++) {
        //polygon.setPoint(i, SFMLDraw::B2VecToSFVec(vertices[i]));
        sf::Vector2f transformedVec = Utils::B2VecToSFVec(vertices[i]);
        polygon.setPoint(i, sf::Vector2f(std::floor(transformedVec.x), std::floor(
                transformedVec.y))); // flooring the coords to fix distorted lines on flat surfaces
    }                                                                                                   // they still show up though.. but less frequently
    polygon.setOutlineThickness(-1.f);
    polygon.setFillColor(Utils::b2ColorToSFML(color, 60));
    polygon.setOutlineColor(Utils::b2ColorToSFML(color));
    m_window->draw(polygon);
}

void DebugDraw::DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color) {
    sf::CircleShape circle(radius * sfdd::SCALE);
    circle.setOrigin(radius * sfdd::SCALE, radius * sfdd::SCALE);
    circle.setPosition(Utils::B2VecToSFVec(center));
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(-1.f);
    circle.setOutlineColor(Utils::b2ColorToSFML(color));

    m_window->draw(circle);
}

void DebugDraw::DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color) {
    sf::CircleShape circle(radius * sfdd::SCALE);
    circle.setOrigin(radius * sfdd::SCALE, radius * sfdd::SCALE);
    circle.setPosition(Utils::B2VecToSFVec(center));
    circle.setFillColor(Utils::b2ColorToSFML(color, 60));
    circle.setOutlineThickness(1.f);
    circle.setOutlineColor(Utils::b2ColorToSFML(color));

    b2Vec2 endPoint = center + radius * axis;
    sf::Vertex line[2] =
            {
                    sf::Vertex(Utils::B2VecToSFVec(center), Utils::b2ColorToSFML(color)),
                    sf::Vertex(Utils::B2VecToSFVec(endPoint), Utils::b2ColorToSFML(color)),
            };

    m_window->draw(circle);
    m_window->draw(line, 2, sf::Lines);
}

void DebugDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) {
    sf::Vertex line[] =
            {
                    sf::Vertex(Utils::B2VecToSFVec(p1), Utils::b2ColorToSFML(color)),
                    sf::Vertex(Utils::B2VecToSFVec(p2), Utils::b2ColorToSFML(color))
            };

    m_window->draw(line, 2, sf::Lines);
}

void DebugDraw::DrawTransform(const b2Transform &xf) {
    float lineLength = 0.4;

    /*b2Vec2 xAxis(b2Vec2(xf.p.x + (lineLength * xf.q.c), xf.p.y + (lineLength * xf.q.s)));*/
    b2Vec2 xAxis = xf.p + lineLength * xf.q.GetXAxis();
    sf::Vertex redLine[] =
            {
                    sf::Vertex(Utils::B2VecToSFVec(xf.p), sf::Color::Red),
                    sf::Vertex(Utils::B2VecToSFVec(xAxis), sf::Color::Red)
            };

    // You might notice that the ordinate(Y axis) points downward unlike the one in Box2D testbed
    // That's because the ordinate in SFML coordinate system points downward while the OpenGL(testbed) points upward
    /*b2Vec2 yAxis(b2Vec2(xf.p.x + (lineLength * -xf.q.s), xf.p.y + (lineLength * xf.q.c)));*/
    b2Vec2 yAxis = xf.p + lineLength * xf.q.GetYAxis();
    sf::Vertex greenLine[] =
            {
                    sf::Vertex(Utils::B2VecToSFVec(xf.p), sf::Color::Green),
                    sf::Vertex(Utils::B2VecToSFVec(yAxis), sf::Color::Green)
            };

    m_window->draw(redLine, 2, sf::Lines);
    m_window->draw(greenLine, 2, sf::Lines);
}

void DebugDraw::DrawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count) {
    if (count > 500) {
        sf::Color color = Utils::b2ParticleColorToSFML(colors[0]);
        auto *v = new sf::Vertex[count];
        for (int i = 0; i < count; i++) {
            v[i].position = Utils::B2VecToSFVec(centers[i]);
            v[i].color = color;
        }
        m_window->draw(v, static_cast<size_t >(count), sf::PrimitiveType::Points);
        free(v);
    } else {
        for (int i = 0; i < count; i++) {
            sf::CircleShape shape(radius * sfdd::SCALE);
            shape.setOrigin(radius * sfdd::SCALE, radius * sfdd::SCALE);
            shape.setPosition(Utils::B2VecToSFVec(centers[i]));
            shape.setFillColor(Utils::b2ParticleColorToSFML(colors[i].GetColor()));
            // shape.setFillColor(sf::Color::Transparent);
            shape.setOutlineThickness(-1.f);
            shape.setOutlineColor(Utils::b2ParticleColorToSFML(colors[i].GetColor()));

            m_window->draw(shape);
        }
    }
}