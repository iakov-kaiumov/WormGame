#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "view/Draw.h"
#include "controller/Game.h"

int main() {
    Game game(b2Vec2(0.0f, 9.0f), sf::VideoMode::getDesktopMode(), "MONSTER GAME");
    game.setCustomDraw(new Draw());
    game.run();
    return 0;
}