#include "World.h"
#include <iostream>

World::World(int sizeX, int sizeY, int size,
        const sf::Color &lc, const sf::Color &dc):
liveColor{lc},
deadColor{dc}
{
    vertices.resize(sizeX*sizeY*4);
    vertices.setPrimitiveType(sf::Quads);
    for(int i = 0; i < sizeX; ++i)
    {
        for(int j = 0; j < sizeY; ++j)
        {
            
            sf::Vertex* quad = &vertices[(i+sizeX*j)*4];

            quad[0].position = sf::Vector2f(i*(size+1), j*(size+1));
            quad[1].position = sf::Vector2f(i*(size+1)+size, j*(size+1));
            quad[2].position = sf::Vector2f(i*(size+1)+size, j*(size+1)+size);
            quad[3].position = sf::Vector2f(i*(size+1), j*(size+1)+size);

            quad[0].color = deadColor;
            quad[1].color = deadColor;
            quad[2].color = deadColor;
            quad[3].color = deadColor;
        }
    }
}

void World::draw(sf::RenderTarget &target, sf::RenderStates states)const
{
    states.transform *= getTransform();
    target.draw(vertices, states);
}

//updates color depending on cellData
void World::update(const std::vector<bool> &cellData)
{
    for(int i = 0; i < cellData.size(); ++i)
    {
        cellData[i] == true ? changeCellColor(i, liveColor) :
            changeCellColor(i, deadColor);
    } 
}


void World::changeCellColor(int index, const sf::Color &c)
{
    sf::Vertex* quad = &vertices[index*4];
    quad[0].color = c;
    quad[1].color = c;
    quad[2].color = c;
    quad[3].color = c;    
}
