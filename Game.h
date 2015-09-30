#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>

class Game{
public:
    Game(int w, int h, int x, int y, int cSize = 10, 
          const sf::Color &oc = sf::Color{150,150,150,},
          const sf::Color &lc = sf::Color::Black,
          const sf::Color &dc = sf::Color::White);
    ~Game();
    
    void run();
    void updateCells();
    void getNewCellData();
    void tick(sf::Time t);
    void getInput();
    void draw();

    int liveNeighbourCount(int cellIndex);
private:
    int dimX, dimY;
    int cellSize;

    sf::Color outlineColor;
    sf::Color liveColor;
    sf::Color deadColor;

    std::vector<sf::RectangleShape> cells;
    std::vector<bool> cellData;
    std::vector<bool> newCellData;


    sf::RenderWindow win;
    sf::View view;
    bool paused;
    sf::Clock perfClock;
    
    float cameraSpeed = 800;
};
