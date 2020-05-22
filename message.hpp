#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "util.hpp"
#include <SFML/Network.hpp>

enum class MsgType: sf::Uint8
{
    Undefined = 0,
    Build = 1,
    Move = 2,
    Place = 4,
    Rand = 8,
    Register = 16,
    Select = 32
};

sf::Packet& operator << (sf::Packet& packet, const MsgType mtype);
sf::Packet& operator >> (sf::Packet& packet, MsgType& mtype);

//Message Model
struct Msg
{
    MsgType msgtype;
    sf::Uint32 id;
    char data[216];
};

struct MsgReg
{
    MsgType msgtype = MsgType::Register;
    sf::Uint32 id = 0;
    std::string username;
    MsgReg(){}
    MsgReg(sf::Uint32 id_, std::string s): id(id_), username(s){}
};

sf::Packet& operator << (sf::Packet& packet, const MsgReg msg);
sf::Packet& operator >> (sf::Packet& packet, MsgReg& msg);

struct MsgSelect
{
    MsgType msgtype = MsgType::Select;
    sf::Uint32 id = 0;
    sf::Uint8 builderID = 0;
    MsgSelect(){}
    MsgSelect(sf::Uint32 id_, sf::Uint8 builderID_): id(id_), builderID(builderID_){}
};

sf::Packet& operator << (sf::Packet& packet, const MsgSelect msg);
sf::Packet& operator >> (sf::Packet& packet, MsgSelect& msg);

struct MsgPos
{
    MsgType msgtype = MsgType::Undefined;
    sf::Uint32 id = 0;
    sf::Uint8 builderID = 0;
    sf::Int8 x = 0;
    sf::Int8 y = 0;
    MsgPos(){}
    MsgPos(MsgType msgtype_, sf::Uint32 id_, sf::Vector2i pos): msgtype(msgtype_), id(id_), x(pos.x), y(pos.y){}
};

sf::Packet& operator << (sf::Packet& packet, const MsgPos msg);
sf::Packet& operator >> (sf::Packet& packet, MsgPos& msg);

struct MsgRand
{
    MsgType msgtype = MsgType::Rand;
    sf::Uint32 id = 0;
    sf::Uint32 r = 0;
    MsgRand(){}
    MsgRand(sf::Uint32 id_, sf::Uint32 r_): id(id_), r(r_){}
};

sf::Packet& operator << (sf::Packet& packet, const MsgRand msg);
sf::Packet& operator >> (sf::Packet& packet, MsgRand& msg);

void readMsg(Message m, Msg& msg);

#endif