#include "tile.hpp"
#include "colours.hpp"

tile::tile():
    pos(0,0),
    level(0)
{
}

void tile::draw(sf::RenderWindow &w)
{
    sf::RectangleShape rectangle;
    rectangle.setOutlineThickness(5);
    sf::Color cOutline;
    sf::Color cFill;
    cOutline = col_outline;

    if (level == 0) {
        cFill = sf::Color(0x76, 0x9D, 0x66, 0xff);
        rectangle.setSize(sf::Vector2f(110, 110));
        rectangle.setPosition(pos.x*120+5, pos.y*120+5);
        rectangle.setFillColor(col_level0);
        w.draw(rectangle);
    }
    else {
        int limit = (level<4)?level:3;
        for (int i = 0; i < limit; i++) {
            int s = 110 - (i * 20);
            rectangle.setSize(sf::Vector2f(s, s));
            rectangle.setPosition((pos.x*120)+(i*10)+5, (pos.y*120)+(i*10)+5);
            rectangle.setOutlineColor(cOutline);
            rectangle.setFillColor(col_level1);
            w.draw(rectangle);
        }
    }
    if (level == 4) {
        sf::CircleShape circle(40);
        circle.setPosition(pos.x*120+20, pos.y*120+20);
        circle.setFillColor(col_dome);
        w.draw(circle);
    }
}

bool tile::build()
{
    std::cout << "Build: " << pos.x << " " << pos.y << " " << level << std::endl;
    if (level < 4) {
        level++;
        return true;
    }
    return false;
}
