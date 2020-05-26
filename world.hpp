#include <SFML/Graphics.hpp>
#include <array>
#include "tile.hpp"
#include "builder.hpp"

#include "network.hpp"
#include "queue.hpp"

#ifndef WORLD_HPP
#define WORLD_HPP

class game;

enum class WorldState
{
    Build,
    Move,
    Place,
    Select
};

class world {
    public:
        world(sf::RenderWindow& w, game *g);
        ~world();
        void update();
        void draw();
        void setup();

    private:
        game *g;
        sf::RenderWindow& window;

        Queue<Message> queue;
        Network network;
        sf::Uint32 id;

        std::array<std::array<tile, 5>, 5> tiles{{{{tile(), tile(), tile()}}, {{tile(), tile(), tile()}}, {{tile(), tile(), tile()}}}} ;
        std::vector<builder> builders;
        unsigned short turn;
        unsigned short buildersSoFar;
        unsigned int selectedBuilderIndex;
        WorldState state = WorldState::Place;

        void build(const sf::Vector2i& pos, bool toSend = true);
        void move(const sf::Vector2i& pos, bool toSend = true);
        void place(const sf::Vector2i& pos, bool toSend = true);
        void select(const sf::Vector2i& pos, bool toSend = true);

        bool noBuilder(const sf::Vector2i& pos);
        bool noDome(const sf::Vector2i& pos);
        bool nearSelectedBuilder(const sf::Vector2i& pos);
        int levelDiff(const sf::Vector2i& pos);

        void processBuild(const Msg& msg);
        void processMove(const Msg& msg);
        void processPlace(const Msg& msg);
        void processSelect(const Msg& msg);

};
#endif
