#include <SFML/Graphics.hpp>
#include <vector>


class World : public sf::Drawable, public sf::Transformable
{
public:
    //inits 2D world made of rectangular cells
    World(int sizeX = 100, int sizeY = 100, int size = 10,
            const sf::Color &lc = sf::Color::Black,
            const sf::Color &dc = sf::Color::White);

    //updates color depending on cellData
    void update(const std::vector<bool> &cellData);
private:
    virtual void draw(sf::RenderTarget &targe, sf::RenderStates states)const;
    void changeCellColor(int index, const sf::Color &c);
    sf::VertexArray vertices;
    
    sf::Color liveColor;
    sf::Color deadColor; 
};
