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

int main(int argc, char **argv)
{
    int sizeX = 100;
    int sizeY = 100;
    
    if(argc == 3)
    {
        sscanf(argv[1], "%d", &sizeX); 
        sscanf(argv[2], "%d", &sizeY); 
    }

    Game gameOfLife(WIDTH, HEIGHT, sizeX, sizeY);
    gameOfLife.run();

    return 0;
}
