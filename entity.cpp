#include "entity.hpp"
#include <iostream>

sf::Vector2f entity::getPosition()
{
    return shape->getPosition();
}
