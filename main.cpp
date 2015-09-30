/*
 *  Game of life
 */

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include "Game.h"

const int WIDTH = 1000;
const int HEIGHT = 1000;

const sf::Color outlineColor(150, 150, 150);
const sf::Color liveColor(0, 0, 0);
const sf::Color deadColor(255, 255, 255);

int main()
{
    Game gameOfLife(WIDTH, HEIGHT, 1000, 1000);
    gameOfLife.run();

    return 0;
}
