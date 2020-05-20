#include <iostream>
#include <SFML/Graphics.hpp>
#include "entity.hpp"

#ifndef TILE_HPP
#define TILE_HPP

class tile : public entity {
public:
    tile();
    void draw(sf::RenderWindow &w);
    bool build();
    sf::Vector2i pos;
    unsigned short level = 0;

private:
};

#endif
