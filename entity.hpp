#include <SFML/Graphics.hpp>

#ifndef ENTITY_HPP
#define ENTITY_HPP

class entity {
    private:
    public:
        sf::Shape *shape;
        sf::Vector2f getPosition();
        sf::Color color;
};
#endif
