#include "world.hpp"
#include "game.hpp"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <memory>

world::world(sf::RenderWindow& w, game *g_):
    window(w),
    g(g_),
    queue(),
    network(queue)
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

void world::build(const sf::Vector2i& pos, bool toSend)
{
    //Check if no builder is there 
    //Check if near selected Builder
    //Check if not dome
    if(noBuilder(pos) && nearSelectedBuilder(pos) && noDome(pos))
    {
        if(toSend)
        {
            network.send(MsgPos(MsgType::Build, id, pos));
        }
        tiles[pos.x][pos.y].build();
        state = WorldState::Select;
        turn++;
    }
}

void world::move(const sf::Vector2i& pos, bool toSend)
{

    if (!noBuilder(pos))
    {
        for (size_t i = 0; i < builders.size(); i++)
        {
            if (builders[i].pos == pos && turn % 2 == builders[i].player) //If this is players builder and players turn
            {
                //selectedBuilderIndex = i; //Change Builder
                if (toSend)
                {
                    network.send(MsgSelect(id, i));
                }
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
        if (toSend)
        {
            network.send(MsgPos(MsgType::Move, id, pos));
        }
        sf::Vector2i m = pos - builders[selectedBuilderIndex].pos;
        builders[selectedBuilderIndex].move(m);
        state = WorldState::Build;
    }
}

void world::place(const sf::Vector2i& pos, bool toSend)
{
    //Check if no Builder is there
    if (noBuilder(pos))
    {
        if (toSend)
        {
            network.send(MsgPos(MsgType::Place, id, pos));
        }
        builders.push_back(builder(pos.x, pos.y, buildersSoFar/2));
        buildersSoFar++;
        if (buildersSoFar == 4)
        {
            state = WorldState::Select;
        }
    }
}

void world::select(const sf::Vector2i& pos, bool toSend)
{
    //Check correct Player
    //Check if Builder has valid move TODO
    for(int i = 0; i < builders.size(); i++)
    {
        if((pos == builders[i].pos) && (turn % 2 == builders[i].player))
        {
            if (toSend)
            {
                network.send(MsgSelect(id, i));
            }
            selectedBuilderIndex = i;
            state = WorldState::Move;
            return;
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
    //Input has been processed
    //Now let's take care of incoming messages
    //The receiver thread would have put the messages in the queue
    Message m;
    queue.pop(m); //non blocking pop
    //If this is an empty packet i.e. there is nothing in the queue, then there is no need to process the message
    if(m.first.endOfPacket())
    {
        return;
    }
    //Otherwise, we need to process the message
    Msg msg;
    readMsg(m, msg);
    //msg now contains the received message
    switch(msg.msgtype)
    {
        case MsgType::Register:
            std::cout << "MsgType::Register\n";
            //Should process the message to add a player
            break;
        case MsgType::Select:
            std::cout << "Msgtype::Select\n";
            processSelect(msg);
            break;
        case MsgType::Move:
            std::cout << "MsgType::Move\n";
            processMove(msg);
            break;
        case MsgType::Build:
            std::cout << "MsgType::Build\n";
            processBuild(msg);
            break;
        case MsgType::Rand:
            std::cout << "MsgType::Rand\n";
            break;
        case MsgType::Place:
            std::cout << "MsgType::Place\n";
            processPlace(msg);
            break;
        default:
            std::cout << "MsgType::Undefined";
            break;
    }

}

void world::processBuild(const Msg& msg)
{
    MsgPos m_pos;
    std::memcpy(&m_pos, &msg, sizeof(MsgPos));
    sf::Vector2i pos(m_pos.x, m_pos.y);
    build(pos, false);
}

void world::processMove(const Msg& msg)
{
    MsgPos m_pos;
    std::memcpy(&m_pos, &msg, sizeof(MsgPos));
    sf::Vector2i pos(m_pos.x, m_pos.y);
    move(pos, false);
}

void world::processPlace(const Msg& msg)
{
    MsgPos m_pos;
    std::memcpy(&m_pos, &msg, sizeof(MsgPos));
    sf::Vector2i pos(m_pos.x, m_pos.y);
    place(pos, false);
}

void world::processSelect(const Msg& msg)
{
    MsgSelect m_select;
    std::memcpy(&m_select, &msg, sizeof(MsgSelect));
    //We may need to call select(pos, false) in the future
    selectedBuilderIndex = static_cast<int>(m_select.builderID);
    state = WorldState::Move;
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
