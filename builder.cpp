#include "builder.hpp"
#include <iostream>

#include "colours.hpp"

builder::builder(unsigned short x_, unsigned short y_, unsigned short p_):
    pos(x_, y_),
    player(p_)
{
}

void builder::draw(sf::RenderWindow &window)
{
    sf::CircleShape circle(40, 40);
    sf::Color fColor = col_player1;
    if (player == 1) {
        fColor = col_player2;
    }
    circle.setFillColor(fColor);
    circle.setPosition(pos.x * 120 + 20, pos.y * 120 + 20);
    window.draw(circle);
}

void builder::move(sf::Vector2i m)
{
    pos += m;
}
