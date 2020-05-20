#include <SFML/Graphics.hpp>
#include "entity.hpp"

#ifndef BUILDER_HPP
#define BUILDER_HPP

class builder : public entity {
public:
    builder(unsigned short x, unsigned short y, unsigned short player);
    unsigned short player;
    void draw(sf::RenderWindow &window);
    void move(sf::Vector2i m);
    sf::Vector2i pos;
private:
};

#endif
