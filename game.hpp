#include <SFML/Graphics.hpp>
#include "world.hpp"

#ifndef GAME_HPP
#define GAME_HPP

enum class gamestate {
    play,
    victory,
    defeat,
    terminate
};

class game {
    private:
        sf::RenderWindow window;
        world *w;
        void play();
        void victory();
        void defeat();
    public:
        gamestate state = gamestate::play;
        game();
        ~game();
        void run();
};
#endif
