/*
 *  Game of life
 */

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

const int WIDTH = 1000;
const int HEIGHT = 1000;
const std::string TITLE = "Game of life";

// returns a 2d vector of next generation 
std::vector<std::vector<sf::RectangleShape>> advanceCells(std::vector<std::vector<sf::RectangleShape>> &cells);

//counts how many live neughbours a cell has
int cellNeighbourCount(std::vector<std::vector<sf::RectangleShape>> &cells, int x, int y);

// checks if g1 and g2 generations are equal
bool equal(std::vector<std::vector<sf::RectangleShape>> &g1, std::vector<std::vector<sf::RectangleShape>> & g2);

int main()
{
    sf::RenderWindow win(sf::VideoMode(WIDTH, HEIGHT), TITLE + "\tPAUSED");
    win.setVerticalSyncEnabled(true);
    
    const int dimX = 100;
    const int dimY = 100;

    std::vector<std::vector<sf::RectangleShape>> cells;
    cells.resize(dimX);
    for(int i = 0; i < dimX; ++i)
    {
        cells[i].resize(dimY); 
    } 

    int cellSizeX = WIDTH / dimX;
    int cellSizeY = HEIGHT / dimY;

  //  std::cout << sizeof(sf::RectangleShape) << "\n";
    
    for(int i = 0; i < dimX; ++i)
    {
        for(int j = 0; j < dimY; ++j)
        {
            cells[i][j].setSize(sf::Vector2f(cellSizeX, cellSizeY));
            cells[i][j].setPosition(i*cellSizeX, j*cellSizeY);
            cells[i][j].setOutlineColor(sf::Color(100, 100, 100));
            cells[i][j].setOutlineThickness(1);
            cells[i][j].setFillColor(sf::Color::White);
        }
    }

    //for checking if generations are equal
    auto lastCells = cells;
          
    bool paused = true; 
    float speed = 5;    //tells how many times a second new generation is calculated
    sf::Time delayTime = sf::milliseconds(1000/speed);  //actual delay time

    int generationCount = 0;
    std::string additionalTitle;
    
    while(win.isOpen())
    {
        // simple event loop;
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

                        if(cells[x/cellSizeX][y/cellSizeY].getFillColor() == sf::Color::White)
                            cells[x/cellSizeX][y/cellSizeY].setFillColor(sf::Color::Black);
                        else
                            cells[x/cellSizeX][y/cellSizeY].setFillColor(sf::Color::White);

                    }
                    break;

                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::Space)
                    {
                        //Toggle paused variable using xor operation
                        paused ^= true;
                    }

                    if(event.key.code == sf::Keyboard::W)
                    {
                        speed += 0.5;
                    }
                    if(event.key.code == sf::Keyboard::S)
                    {
                        if(speed > 1)
                            speed -= 0.5;
                    }
                    if(event.key.code == sf::Keyboard::Escape)
                    {
                        // Clear all the cells and pause 
                        for(int i = 0; i < cells.size(); ++i)
                        {
                            for(int j = 0; j < cells[i].size(); ++j)
                            {
                                cells[i][j].setFillColor(sf::Color::White);
                            }
                        }
                        paused = true;
                        generationCount = 0;
                    }
                    break;
                default:
                    break;

            }
        }

        //Logic part
        delayTime = sf::milliseconds(1000/speed);
        if(!paused)
        {
            lastCells = cells;
            cells = advanceCells(cells); 
            if( !equal(cells, lastCells))
                generationCount++;

            additionalTitle = "\tGenerations: " + std::to_string(generationCount);
            win.setTitle(TITLE + additionalTitle);
            sf::sleep(delayTime);
        } 
        else {
            additionalTitle = "\tGenerations: " + std::to_string(generationCount) + "\tPAUSED";
            win.setTitle(TITLE + additionalTitle);
        }


        //Drawing part
        win.clear();
        for(int i = 0; i < dimX; ++i)
        {
            for(int j = 0; j < dimY; ++j)
            {
                win.draw(cells[i][j]);
            }
        }
        win.display();
    }


    return 0;
}

// returns a 2d vector of cells of next generation
std::vector<std::vector<sf::RectangleShape>> advanceCells(std::vector<std::vector<sf::RectangleShape>> &cells)
{
    std::vector<std::vector<sf::RectangleShape>> newCells;
    newCells.resize(cells.size());
    for(int i = 0; i < newCells.size(); ++i)
        newCells[i].resize(cells[i].size());
    newCells = cells;
    //Rules:
    //1. Any live cell with fewer than two live neighbours dies
    //2. Any live cell with two or three live neighbours lives next generation
    //3. Any live cell with more than three live neighbours dies
    //4. Any dead cell with exactly three live neighbours comes to life
    for(int i = 0; i < cells.size(); ++i)
    {
        for(int j = 0; j < cells[i].size(); j++)
        {
            int neighbours = cellNeighbourCount(cells, i, j);
            //Tests for live cells
            if(cells[i][j].getFillColor() == sf::Color::Black)
            {
                //1st rule
                if( neighbours < 2)
                    newCells[i][j].setFillColor(sf::Color::White);

                //3rd rule
                if(neighbours > 3)
                    newCells[i][j].setFillColor(sf::Color::White);
            } 
            // 4th rule
            else if(neighbours == 3)
                newCells[i][j].setFillColor(sf::Color::Black);
        }
    }

    return newCells; 
}

//counts how many live neughbours a cell has
int cellNeighbourCount(std::vector<std::vector<sf::RectangleShape>> &cells, int x, int y)
{
    int count = 0; 
        
    for(int i = x-1; i < x+2; ++i)
    {
        for(int j = y-1; j < y+2; ++j)
        {
            if(i < 0 || i >= cells.size() || j < 0 || j >= cells[0].size() || (i == x && j == y))
                continue;

            if(cells[i][j].getFillColor() == sf::Color::Black)
                ++count;
        }
    }
    
    return count;
}
// checks if g1 and g2 generations are equal
bool equal(std::vector<std::vector<sf::RectangleShape>> &g1, std::vector<std::vector<sf::RectangleShape>> & g2)
{
    bool equ = true;
    for( int i = 0; i < g1.size(); ++ i)
    {
        for(int j = 0; j < g1[i].size(); ++j)
        {
            equ &= g1[i][j].getFillColor() == g2[i][j].getFillColor();
            //equ &= (g1[i][j].getFillColor() == sf::Color::White && g2[i][j].getFillColor() == sf::Color::White)
            //    || (g1[i][j].getFillColor() == sf::Color::Blue && g2[i][j].getFillColor() == sf::Color::Black);
        }
    }
    return equ;
}
