#include <sstream>
#include <iostream>
#include "Game.h"
#include "../model/QueryCallback.h"
#include "../utils/Utils.h"


Game::Game(b2Vec2 gravity,
           sf::VideoMode videoMode,
           std::string windowTitle) : model(gravity), window(videoMode, windowTitle) {

    window.setFramerateLimit(120);

    // Init world objects
    sf::Vector2f center = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);

    model.createGroundBox(Utils::sfIntToB2Float(window.getSize().x), Utils::sfIntToB2Float(window.getSize().y));

    model.createOcean(Utils::sfVecToB2Vec(center + sf::Vector2f(110, 200)));

    stirrer = new Stirrer(&model.world,
            b2Vec2(Utils::sfIntToB2Float(window.getSize().x) - 5.f, Utils::sfIntToB2Float(window.getSize().y) / 2), 0.6f,
                          Utils::sfIntToB2Float(window.getSize().y) - 2.3f);

    worm = model.createElasticBody(b2Vec2(Utils::sfIntToB2Float(window.getSize().x) / 2,
            Utils::sfIntToB2Float(window.getSize().y) / 1.5f));

    sound.waves(true);
}

/***
 * Call to start main game loop
 * Handles window events while window is open
 */
void Game::run() {
    sf::Event event;
    while (window.isOpen()) {
        sf::Time deltaTime = deltaClock.restart();
        while (window.pollEvent(event)) {
            handleEvents(event);
        }
        if (stirrer != nullptr) {
            stirrer->move();
        }
        handleBoats();
        model.step();

        if (debugDraw != nullptr)
            window.clear();
            model.world.DrawDebugData();
            window.display();
        if (draw != nullptr) {
            gameInfo.score = score;
            gameInfo.time = deltaTime;
            draw->step(&model.world, &gameInfo);
        }
    }
}

/**
 * Adding and deleting boats
 */
void Game::handleBoats() {
    // Adding new boats if necessary
    if (boats.size() < round(sqrt(score / 3 + 1))) {
        b2Vec2 pos = b2Vec2(Utils::sfIntToB2Float(random.iRandom(200, window.getSize().x - 400)),
                            Utils::sfIntToB2Float(window.getSize().y) / 3.f);
        unsigned int r = random.iRandom(1, 6);
        if (r == 2)
            boats.emplace_back(Boat(&model.world, pos, BATTLESHIP_FIXTURE_PATH, BATTLESHIP_SPRITE_PATH));
        else
        if (r == 3)
            boats.emplace_back(Boat(&model.world, pos, SUBMARINE_FIXTURE_PATH, SUBMARINE_SPRITE_PATH));
        else
            boats.emplace_back(Boat(&model.world, pos, BOAT_FIXTURE_PATH, BOAT_SPRITE_PATH));
    }
    // Delete boat if necessary
    int i = 0;
    while (i < boats.size()) {
        if (boats[i].checkRotation()) {
            boats[i].body->SetUserData(nullptr);
            model.world.DestroyBody(boats[i].body);
            boats.erase(boats.begin() + i);
            score++;
        }
        i++;
    }
}

/**
 * Handling window event
 * @param event RenderWindow event
 */
void Game::handleEvents(sf::Event event) {
    if (event.type == sf::Event::Closed ||
        (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
        window.close();
    else
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left)
            if (worm != nullptr) {
                worm->ApplyForce(1, 5, b2Vec2(30, 0));
                worm->ApplyForce(20, 25, b2Vec2(-100, 0));
            }
        if (event.key.code == sf::Keyboard::Right)
            if (worm != nullptr) {
                worm->ApplyForce(1, 5, b2Vec2(30, 0));
                worm->ApplyForce(20, 25, b2Vec2(100, 0));
            }
        if (event.key.code == sf::Keyboard::Up)
            if (worm != nullptr)
                worm->ApplyForce(1, 5, b2Vec2(0, -200));

        switch (event.key.code) {
            case sf::Keyboard::F1:
                if (debugDraw != nullptr) {
                    if (debugDraw->GetFlags() & b2Draw::e_shapeBit)
                        debugDraw->ClearFlags(b2Draw::e_shapeBit);
                    else
                        debugDraw->AppendFlags(b2Draw::e_shapeBit);
                }
                break;
            case sf::Keyboard::F2:
                if (debugDraw != nullptr) {
                    if (debugDraw->GetFlags() & b2Draw::e_aabbBit)
                        debugDraw->ClearFlags(b2Draw::e_aabbBit);
                    else
                        debugDraw->AppendFlags(b2Draw::e_aabbBit);
                }
                break;
            case sf::Keyboard::F3:
                if (debugDraw != nullptr) {
                    if (debugDraw->GetFlags() & b2Draw::e_centerOfMassBit)
                        debugDraw->ClearFlags(b2Draw::e_centerOfMassBit);
                    else
                        debugDraw->AppendFlags(b2Draw::e_centerOfMassBit);
                }
                break;
            case sf::Keyboard::Num1:
                model.addBody(model.createSquare(Utils::sfVecToB2Vec(sf::Mouse::getPosition(window))));
                break;
            case sf::Keyboard::Num2:
                model.addBody(model.createCircle(Utils::sfVecToB2Vec(sf::Mouse::getPosition(window))));
                break;
            case sf::Keyboard::BackSpace:
                if (!model.bodies.empty())
                    model.destroyBody(model.bodies.back());
                break;
            default:
                break;
        }
    } else
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left &&
        model.mouseJoint == nullptr) {
        b2Vec2 mousePos = Utils::sfVecToB2Vec(sf::Mouse::getPosition(window));

        // Make a small box.
        b2AABB aabb;
        b2Vec2 d;
        d.Set(0.001f, 0.001f);
        aabb.lowerBound = mousePos + d;
        aabb.upperBound = mousePos - d;

        // Query the world for overlapping shapes.
        QueryCallback callback(mousePos);
        model.world.QueryAABB(&callback, aabb);

        if (callback.m_fixture) {
            b2Body *body = callback.m_fixture->GetBody();
            b2MouseJointDef md;
            md.bodyA = model.ground; //If bounding box body is used instead, the dynamic bodes can be dragged over the bounding box and we don't want that
            md.bodyB = body;
            md.target = mousePos;
            md.maxForce = 1000.0f * body->GetMass();
            model.mouseJoint = (b2MouseJoint *) model.world.CreateJoint(&md);
            body->SetAwake(true);
        }
    } else if (event.type == sf::Event::MouseMoved && model.mouseJoint != nullptr) {
        b2Vec2 mousePos = Utils::sfVecToB2Vec(sf::Mouse::getPosition(window));
        model.mouseJoint->SetTarget(mousePos);
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left &&
               model.mouseJoint != nullptr) {
        model.world.DestroyJoint(model.mouseJoint);
        model.mouseJoint = nullptr;
    }
}

void Game::enableDebugDraw() {
    debugDraw = new DebugDraw(window);
    model.setDebugDraw(debugDraw);
    /* Set initial flags for what to draw */
    debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_particleBit);
}

void Game::disableDebugDraw() {
    model.setDebugDraw(nullptr);
    debugDraw = nullptr;
}

void Game::setCustomDraw(BasicDraw *_draw) {
    draw = _draw;
    draw->init(&window);
}
