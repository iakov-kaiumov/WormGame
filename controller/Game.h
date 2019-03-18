#ifndef GAME_H
#define GAME_H


#include "../model/Model.h"
#include "../vehicles/Boat.h"
#include "../model/Stirrer.h"
#include "../view/BasicDraw.h"
#include "../sound/Sound.h"
#include "../utils/MyRandom.h"
#include "GameInfo.h"

class Game {
public:
    Game(b2Vec2 gravity, sf::VideoMode videoMode, std::string windowTitle);
    void run();
    void enableDebugDraw();
    void disableDebugDraw();
    void setCustomDraw(BasicDraw *_draw);
private:
    void handleBoats();
    void handleEvents(sf::Event event);
    GameInfo gameInfo;
    MyRandom random;
    Sound sound;
    Model model;
    DebugDraw *debugDraw = nullptr;
    BasicDraw *draw = nullptr;
    std::vector<Boat> boats;
    Stirrer *stirrer = nullptr;
    b2ParticleSystem *worm = nullptr;
    sf::RenderWindow window;
    sf::Clock deltaClock;
    int score = 0;
    // Const
    const char *BOAT_FIXTURE_PATH = "vehicles/boat.xml";
    const char *BOAT_SPRITE_PATH = "vehicles/boat.png";
    const char *BATTLESHIP_FIXTURE_PATH = "vehicles/battleship.xml";
    const char *BATTLESHIP_SPRITE_PATH = "vehicles/battleship.png";
    const char *SUBMARINE_FIXTURE_PATH = "vehicles/submarine.xml";
    const char *SUBMARINE_SPRITE_PATH = "vehicles/submarine.png";
};


#endif //GAME_H
