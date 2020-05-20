#include "game.hpp"
#include <iostream>

game::game():
    window(sf::VideoMode(800, 600), "Santorini")
{
    w = new world(window, this);
}

game::~game()
{
    delete w;
}

void game::play()
{
    w->update();
    w->draw();
}

void game::victory()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        state = gamestate::play;
        delete w;
        w = new world(window, this);
        w->setup();
    }
}

void game::defeat()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        state = gamestate::play;
        delete w;
        w = new world(window, this);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        std::cout << "Terminate" << std::endl;
        state = gamestate::terminate;
    }
}

void game::run()
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        switch (state) {
            case gamestate::play:
                play();
                break;
            case gamestate::victory:
                victory();
                break;
            case gamestate::defeat:
                defeat();
                break;
            case gamestate::terminate:
                return;
            default:
                break;       
        }
        window.display();
    }
}
