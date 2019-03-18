#include <iostream>
#include "Draw.h"
#include "../utils/Utils.h"
#include "../vehicles/BodyTextureData.h"
#include "../vehicles/Body.h"

void Draw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const sf::Color& fillColor, const sf::Color& lineColor) {
    sf::ConvexShape polygon(static_cast<size_t>(vertexCount));
    for (size_t i = 0; i < vertexCount; i++) {
        polygon.setPoint(i, Utils::B2VecToSFVec(vertices[i]));
    }
    polygon.setOutlineThickness(-1.f);
    polygon.setFillColor(fillColor);
    polygon.setOutlineColor(lineColor);
    m_window->draw(polygon);
}

void Draw::DrawCircle(const b2Vec2 &center, float32 radius, const sf::Color& fillColor, const sf::Color& lineColor) {
    sf::CircleShape circle(radius * sfdd::SCALE);
    circle.setOrigin(radius * sfdd::SCALE, radius * sfdd::SCALE);
    circle.setPosition(Utils::B2VecToSFVec(center));
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(-1.f);
    circle.setOutlineColor(lineColor);

    m_window->draw(circle);
}

void Draw::DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis,
        const sf::Color& fillColor, const sf::Color& lineColor) {
    sf::CircleShape circle(radius * sfdd::SCALE);
    circle.setOrigin(radius * sfdd::SCALE, radius * sfdd::SCALE);
    circle.setPosition(Utils::B2VecToSFVec(center));
    circle.setFillColor(fillColor);
    circle.setOutlineThickness(1.f);
    circle.setOutlineColor(lineColor);

    b2Vec2 endPoint = center + radius * axis;
    sf::Vertex line[2] =
            {
                    sf::Vertex(Utils::B2VecToSFVec(center), lineColor),
                    sf::Vertex(Utils::B2VecToSFVec(endPoint), lineColor),
            };

    m_window->draw(circle);
    m_window->draw(line, 2, sf::Lines);
}

void Draw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const sf::Color& lineColor) {
    sf::Vertex line[] =
            {
                    sf::Vertex(Utils::B2VecToSFVec(p1), lineColor),
                    sf::Vertex(Utils::B2VecToSFVec(p2), lineColor)
            };

    m_window->draw(line, 2, sf::Lines);
}

void Draw::DrawTransform(const b2Transform &xf) {
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

float distance(sf::Vertex v1, sf::Vertex v2) {
    return pow(v1.position.x - v2.position.x, 2.f) + pow(v1.position.y - v2.position.y, 2.f);
}

void Draw::DrawParticles(int type, const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count) {
    switch (type) {
        case 0: {
            sf::Color color = Utils::b2ParticleColorToSFML(colors[0]);
            auto *v = new sf::Vertex[count];
            for (int i = 0; i < count; i++) {
                v[i].position = Utils::B2VecToSFVec(centers[i]);
                v[i].color = color;
            }
            m_window->draw(v, static_cast<size_t >(count), sf::PrimitiveType::Points);
            free(v);
        }
            break;
        default: {
            b2ParticleColor c = colors[0];
            sf::Color color = Utils::b2ParticleColorToSFML(c);
            c.a = 100;
            sf::Color tColor = Utils::b2ParticleColorToSFML(c);
            for (int i = 0; i < count; i++) {
                sf::CircleShape shape(radius * sfdd::SCALE);
                shape.setOrigin(radius * sfdd::SCALE, radius * sfdd::SCALE);
                shape.setPosition(Utils::B2VecToSFVec(centers[i]));
                shape.setFillColor(tColor);
                m_window->draw(shape);
            }
        }
    }
}

void Draw::DrawJoint(b2Joint* joint) {
    b2Body* bodyA = joint->GetBodyA();
    b2Body* bodyB = joint->GetBodyB();
    const b2Transform& xf1 = bodyA->GetTransform();
    const b2Transform& xf2 = bodyB->GetTransform();
    b2Vec2 x1 = xf1.p;
    b2Vec2 x2 = xf2.p;
    b2Vec2 p1 = joint->GetAnchorA();
    b2Vec2 p2 = joint->GetAnchorB();

    sf::Color color(125, 204, 204);

    switch (joint->GetType()) {
        case e_distanceJoint:
            DrawSegment(p1, p2, color);
            break;

        case e_pulleyJoint: {
            b2PulleyJoint* pulley = (b2PulleyJoint*)joint;
            b2Vec2 s1 = pulley->GetGroundAnchorA();
            b2Vec2 s2 = pulley->GetGroundAnchorB();
            DrawSegment(s1, p1, color);
            DrawSegment(s2, p2, color);
            DrawSegment(s1, s2, color);
        }
            break;

        default:
            // DrawSegment(x1, p1, color);
            DrawSegment(p1, p2, color);
            // DrawSegment(x2, p2, color);
    }
}

/**
 * Draw any shape
 * @param fixture
 * @param xf
 * @param color
 */
void Draw::DrawShape(b2Fixture* fixture, const b2Transform& xf, const sf::Color& fillColor, const sf::Color& lineColor) {
    switch (fixture->GetType()) {
        case b2Shape::e_circle: {
            b2CircleShape* circle = (b2CircleShape*)fixture->GetShape();

            b2Vec2 center = b2Mul(xf, circle->m_p);
            float32 radius = circle->m_radius;
            b2Vec2 axis = b2Mul(xf.q, b2Vec2(1.0f, 0.0f));

            DrawSolidCircle(center, radius, axis, fillColor, lineColor);
        }
            break;

        case b2Shape::e_edge: {
            b2EdgeShape* edge = (b2EdgeShape*)fixture->GetShape();
            b2Vec2 v1 = b2Mul(xf, edge->m_vertex1);
            b2Vec2 v2 = b2Mul(xf, edge->m_vertex2);
            DrawSegment(v1, v2, lineColor);
        }
            break;

        case b2Shape::e_chain: {
            b2ChainShape* chain = (b2ChainShape*)fixture->GetShape();
            int32 count = chain->m_count;
            const b2Vec2* vertices = chain->m_vertices;

            b2Vec2 v1 = b2Mul(xf, vertices[0]);
            for (int32 i = 1; i < count; ++i)
            {
                b2Vec2 v2 = b2Mul(xf, vertices[i]);
                DrawSegment(v1, v2, lineColor);
                DrawCircle(v1, 0.05f, fillColor, lineColor);
                v1 = v2;
            }
        }
            break;

        case b2Shape::e_polygon: {
            b2PolygonShape* poly = (b2PolygonShape*)fixture->GetShape();
            int32 vertexCount = poly->m_count;
            b2Assert(vertexCount <= b2_maxPolygonVertices);
            b2Vec2 vertices[b2_maxPolygonVertices];

            for (int32 i = 0; i < vertexCount; ++i) {
                vertices[i] = b2Mul(xf, poly->m_vertices[i]);
            }

            DrawSolidPolygon(vertices, vertexCount, fillColor, lineColor);
        }
            break;

        default:
            break;
    }
}


void Draw::step(b2World *world, GameInfo *gameInfo) {
    // Clear window
    m_window->clear(BACKGROUND_COLOR);
    // Draw bodies
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
        auto *pTextureData = (BodyTextureData*) b->GetUserData();
        const b2Transform& xf = b->GetTransform();
        int i = 0;
        // Draw sprite if possible
        if (pTextureData != nullptr && pTextureData->sprite != nullptr) {
            pTextureData->sprite->setPosition(Utils::B2VecToSFVec(b->GetPosition()));
            pTextureData->sprite->setRotation(b->GetAngle() * 180 / b2_pi);
            m_window->draw(*pTextureData->sprite);
            continue;
        }
        for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
            if (!b->IsActive()) {
                DrawShape(f, xf, sf::Color(125, 125, 70, 200), sf::Color(125, 125, 70));
            } else
            if (pTextureData != nullptr) {
                DrawShape(f, xf, pTextureData->fillColor, pTextureData->lineColor);
            } else
            if (b->GetType() == b2_staticBody) {
                DrawShape(f, xf, sf::Color(125, 230, 125, 200), sf::Color(125, 230, 125));
            } else
            if (b->GetType() == b2_kinematicBody) {
                DrawShape(f, xf, sf::Color(125, 125, 230, 200), sf::Color(125, 125, 230));
            } else {
                DrawShape(f, xf, sf::Color(230, 180, 180, 200), sf::Color(230, 180, 180));
            }
            i++;
        }
    }
    // Draw joint
    for (b2Joint* j = world->GetJointList(); j; j = j->GetNext()) {
        DrawJoint(j);
    }

    // Draw particle systems
    for (b2ParticleSystem* s = world->GetParticleSystemList(); s; s = s->GetNext()) {
        DrawParticles(s->GetParticleFlags(0), s->GetPositionBuffer(), s->GetRadius(),
                s->GetColorBuffer(), s->GetParticleCount());
    }

    // Draw text
    stream.precision(0);
    stream << std::fixed << "FPS: " << 1.f / gameInfo->time.asSeconds();
    fpsCounter.setString(stream.str());
    m_window->draw(fpsCounter);
    stream.str("");

    helpText.setString(std::string("Score: ") + std::to_string(gameInfo->score));
    m_window->draw(helpText);
}

void Draw::init(sf::RenderWindow *window) {
    m_window = window;
    /* Initialize font */
    if (!mainFont.loadFromFile("Franchise-Bold-hinted.ttf")) {
        exit(1);
    }

    /* Initialize debug text */
    fpsCounter.setFont(mainFont);
    fpsCounter.setFillColor(sf::Color::White);

    helpText.setFont(mainFont);
    helpText.setFillColor(sf::Color::White);
    helpText.setCharacterSize(30);
    helpText.setPosition(static_cast<float>(m_window->getSize().x) - static_cast<float>(m_window->getSize().x / 2.f), 0.f);

    helpText.setString(std::string("Score: 0"));
}
