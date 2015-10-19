#include "Game.h"

Game::Game(int w, int h, int x, int y, int cSize,
        const sf::Color &lc, const sf::Color &dc):
    dimX{x}, dimY{y},
    cellSize{cSize},
    cellData(x*y, false),
    newCellData(x*y, false),
    world{x, y, cSize, lc, dc},
    win(sf::VideoMode(w, h), "Game of life")
{
}

Game::~Game()
{

}

void Game::run()
{
    paused = true;
    sf::Time timePerFrame = sf::milliseconds(1000/60);
    sf::Clock c; 
    sf::Time delta;
    clock.restart(); 
    while(win.isOpen())
    {
        c.restart();
        getInput();
        tick(timePerFrame);
        draw();
        delta = timePerFrame - c.getElapsedTime();
        if(delta > sf::Time::Zero)
            sf::sleep(delta);
    } 
}

void Game::getInput()
{
        sf::Event event;
        while(win.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    win.close();
                    break;

                case sf::Event::MouseButtonPressed:
                    if(event.mouseButton.button == sf::Mouse::Left)
                    {
                        int x = event.mouseButton.x;
                        int y = event.mouseButton.y;
                        sf::Vector2f coords = win.mapPixelToCoords(sf::Vector2i(x, y));
                        int xpos = coords.x/(cellSize+1);
                        int ypos = coords.y/(cellSize+1);
                        if(xpos <=  dimX && ypos <= dimY && ypos >= 0 && xpos >= 0)
                        {
                            cellData[ypos*dimX+xpos] = cellData[ypos*dimX+xpos] ^ true;
                        }
                    }
                    break;

                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::Space)
                    {
                        //Toggle paused variable using xor operation
                        paused ^= true;
                        std::cout << "PAUSE: " << paused << "\n";
                    }

                    if(event.key.code == sf::Keyboard::E)
                    {
                        rate -= 10;
                    }
                    if(event.key.code == sf::Keyboard::Q)
                    {
                         rate += 10;
                    }
                    if(event.key.code == sf::Keyboard::Escape)
                    {
                        // Clear all the cells and pause 
                        paused = true;
                    }
                    break;

                case sf::Event::Resized:
                    view = sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height));
                    win.setView(view);
                    
                    break;
                default:
                    break;

            }
        }
}

void Game::tick(sf::Time t)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        view.move(0, -cameraSpeed*t.asSeconds());
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        view.move(0, cameraSpeed*t.asSeconds());
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        view.move(-cameraSpeed*t.asSeconds(), 0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        view.move(cameraSpeed*t.asSeconds(), 0);
    }
    win.setView(view);

    if(!paused && clock.getElapsedTime().asMilliseconds() >= rate)
    {
        getNewCellData();
        cellData = newCellData;
        clock.restart();
    }

    world.update(cellData);
}

// Advances cells by 1 generation
void Game::getNewCellData()
{
    int neighbours = 0;
    for(int i = 0; i < cellData.size(); ++i)
    {
        neighbours = liveNeighbourCount(i);
        if(cellData[i] == true)
        {
            if(neighbours < 2)
                newCellData[i] = false;

            if(neighbours == 2 || neighbours == 3)
                newCellData[i] = true; 

            if(neighbours > 3)
                newCellData[i] = false;
        }
        else if (neighbours == 3)
        {
            newCellData[i] = true;
        }
    }
}

//counts how many live neighbour cells a specified cell has
int Game::liveNeighbourCount(int cellIndex)
{
    int count = 0;
    for( int i = 0; i < 3; ++i)
    {
        for(int j = -1; j < 2; ++j)
        {
            int indx = cellIndex+j*dimX-1+i;
            if(indx < 0 ||  indx >= cellData.size() || indx == cellIndex)
                continue;
            
            if(cellData[cellIndex+j*dimX-1+i])
                ++count;
            
        }
    } 

    return count;
}

void Game::draw()
{
        win.clear(sf::Color{50, 50, 50});
        win.draw(world);
        win.display();
}
