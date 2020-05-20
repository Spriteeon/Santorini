#include "world.hpp"
#include "game.hpp"
#include <cstdlib>
#include <iostream>
#include <algorithm>

world::world(sf::RenderWindow& w, game *g_):
    window(w),
    g(g_)
{
    srand(time(0));
    window.setFramerateLimit(60);
    setup();
    buildersSoFar = 0;
}

void world::setup()
{
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            tiles[i][j].pos = sf::Vector2i(i, j);
            tiles[i][j].level = 0;
        }
    }
    turn = 0;
}

world::~world()
{
}

bool world::noBuilder(const sf::Vector2i& pos)
{
    for(builder& b : builders)
    {
        if (b.pos == pos)
        {
            return false;
        }
    }
    return true;
}

bool world::noDome(const sf::Vector2i& pos)
{
    return tiles[pos.x][pos.y].level < 4;
}

bool world::nearSelectedBuilder(const sf::Vector2i& pos)
{
    sf::Vector2i diff = pos - builders[selectedBuilderIndex].pos;
    return (diff.x >= -1 && diff.y >= -1 && 
            diff.x <= 1 && diff.y <= 1 && 
            !(diff.x == 0 && diff.y == 0));
}

int world::levelDiff(const sf::Vector2i& pos)
{
    sf::Vector2i bpos = builders[selectedBuilderIndex].pos;
    return tiles[pos.x][pos.y].level - tiles[bpos.x][bpos.y].level;
}

void world::build(const sf::Vector2i& pos)
{
    //Check if no builder is there 
    //Check if near selected Builder
    //Check if not dome
    if(noBuilder(pos) && nearSelectedBuilder(pos) && noDome(pos))
    {
        tiles[pos.x][pos.y].build();
        state = WorldState::Select;
        turn++;
    }
}

void world::move(const sf::Vector2i& pos)
{

    if (!noBuilder(pos))
    {
        for (size_t i = 0; i < builders.size(); i++)
        {
            if (builders[i].pos == pos && turn % 2 == builders[i].player) //If this is players builder and players turn
            {
                selectedBuilderIndex = i; //Change Builder
                return;
            }
        }
    }
    //Check if no builder is there 
    //Check if near selected Builder
    //Check if not dome
    //Check move up by no more than one level
    if(noBuilder(pos) && nearSelectedBuilder(pos) && noDome(pos) && levelDiff(pos) <= 1)
    {
        sf::Vector2i prevPos = builders[selectedBuilderIndex].pos;
        if (tiles[prevPos.x][prevPos.y].level == 2 && tiles[pos.x][pos.y].level == 3)
        {
            //TODO
            //Instead of checking turn, we want to check if local player has won the game.
            //Could be done easily with a class variable to note who the local player is
            if (turn % 2 == 0)
            {
                g->state = gamestate::victory;
            }
            else
            {
                g->state = gamestate::defeat;
            }
            
        }
        sf::Vector2i m = pos - builders[selectedBuilderIndex].pos;
        builders[selectedBuilderIndex].move(m);
        state = WorldState::Build;
    }
}

void world::place(const sf::Vector2i& pos)
{
    //Check if no Builder is there
    if (noBuilder(pos))
    {
        builders.push_back(builder(pos.x, pos.y, buildersSoFar/2));
        buildersSoFar++;
        if (buildersSoFar == 4)
        {
            state = WorldState::Select;
        }
    }
}

void world::select(const sf::Vector2i& pos)
{
    //Check correct Player
    //Check if Builder has valid move TODO
    for(int i = 0; i < builders.size(); i++)
    {
        if(pos == builders[i].pos)
        {
            if (turn % 2 == builders[i].player)
            {
                selectedBuilderIndex = i;
                state = WorldState::Move;
                return;
            }
        }
    }
}

void world::update()
{
    static bool clicked = false;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !clicked) 
    {
        clicked = true;
        sf::Vector2i pos = sf::Mouse::getPosition(window) / 120;
        switch(state)
        {
            case WorldState::Build:
                std::cout << "Build<n";
                build(pos);
                break;
            case WorldState::Move:
                std::cout << "Move<n";
                move(pos);
                break;
            case WorldState::Place:
                std::cout << "Place<n";
                place(pos);
                break;
            case WorldState::Select:
                std::cout << "Select<n";
                select(pos);
                break;
            default:
                break;
        }
    }
    else
    {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            clicked = false;
        }
    }
}

void world::draw()
{
    window.clear();
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            tiles[i][j].draw(window);
        }
    }
    for (builder& b : builders) {
        b.draw(window);
    }
}
