#include "Game.h"

Game::Game(int w, int h, int x, int y, int cSize, const sf::Color &oc,
        const sf::Color &lc, const sf::Color &dc):
    dimX{x}, dimY{y},
    cellSize{cSize},
    outlineColor{oc},
    liveColor{lc},
    deadColor{dc},
    //cells(x*y),
    cellData(x*y, false),
    newCellData(x*y, false),
    world{x, y},
    win(sf::VideoMode(w, h), "Game of life"),
    statUpdateTime{sf::Time::Zero},
    frames{0}
{
    //Create window
    

    //Place all squares and apply correct colors
    /*
    for(int i = 0; i < dimY; ++i)
    {
        for(int j = 0; j < dimX; ++j)
        {
            cells[i*dimX+j].setSize(sf::Vector2f(cellSize, cellSize));
            cells[i*dimX+j].setPosition(j*cellSize, i*cellSize);
            cells[i*dimX+j].setOutlineColor(outlineColor);
            cells[i*dimX+j].setOutlineThickness(1);
            cells[i*dimX+j].setFillColor(deadColor);
        }
    }
    */
}

Game::~Game()
{

}

void Game::run()
{
    paused = true;
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::milliseconds(1000/60);

    sf::Time updateTime; 
    
    while(win.isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while(timeSinceLastUpdate > timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;

            getInput();
            tick(timePerFrame);
            updateStats(timePerFrame);
        }
        perfClock.restart();
        draw();
        drawTime = perfClock.restart();
    } 
}

void Game::updateStats(sf::Time t)
{
    statUpdateTime += t;
    frames++;
    if(statUpdateTime >= sf::milliseconds(1000))
    {
        std::cout << "FPS: " << frames << " Draw time: " << 
            drawTime.asMicroseconds() << " us" << '\n';
        statUpdateTime -= sf::milliseconds(1000);
        frames = 0;
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
                        //std::cout << liveNeighbourCount(101) << "\n";
         
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
                        //speed += 0.5;
                    }
                    if(event.key.code == sf::Keyboard::Q)
                    {
                        //if(speed > 1)
                         //   speed -= 0.5;
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
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        view.move(0, -cameraSpeed*t.asSeconds());
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        view.move(0, cameraSpeed*t.asSeconds());
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        view.move(-cameraSpeed*t.asSeconds(), 0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        view.move(cameraSpeed*t.asSeconds(), 0);
    }
    win.setView(view);

    if(!paused)
    {
        perfClock.restart(); 
        getNewCellData();
        std::cout << "GetNewCellData: " << perfClock.restart().asMicroseconds() <<
            " us\n";
        cellData = newCellData;
    }
    perfClock.restart();
    world.update(cellData);
    //std::cout << "update world: " << perfClock.restart().asMicroseconds() << 
    //    " us\n";
}

// Paint cells depending on cellData
void Game::updateCells()
{
    for( int i = 0; i < cellData.size(); ++i)
    {
        //cellData[i] ? cells[i].setFillColor(liveColor) : cells[i].setFillColor(deadColor);        
    } 
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
