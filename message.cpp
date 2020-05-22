#include "message.hpp"

#include <iostream>
#include <sstream>
#include <cstring>
#include <memory>

sf::Packet& operator << (sf::Packet& packet, const MsgType mtype)
{
    return packet << static_cast<sf::Uint8>(mtype);
}

sf::Packet& operator >> (sf::Packet& packet, MsgType& mtype)
{
    sf::Uint8 r;
    packet >> r;
    mtype = static_cast<MsgType>(r);
    return packet;
}

sf::Packet& operator << (sf::Packet& packet, const MsgReg msg)
{
    return packet << msg.msgtype << msg.id << msg.username;
}

sf::Packet& operator >> (sf::Packet& packet, MsgReg& msg)
{
    return packet >> msg.msgtype >> msg.id >> msg.username;
}

sf::Packet& operator << (sf::Packet& packet, const MsgSelect msg)
{
    return packet << msg.msgtype << msg.id << msg.builderID;
}

sf::Packet& operator >> (sf::Packet& packet, MsgSelect& msg)
{
    return packet >> msg.msgtype >> msg.id >> msg.builderID;
}

sf::Packet& operator << (sf::Packet& packet, const MsgPos msg)
{
    return packet << msg.msgtype << msg.id << msg.x << msg.y;
}

sf::Packet& operator >> (sf::Packet& packet, MsgPos& msg)
{
    return packet >> msg.msgtype >> msg.id >> msg.x >> msg.y;
}

sf::Packet& operator << (sf::Packet& packet, const MsgRand msg)
{
    return packet << msg.msgtype << msg.id << msg.r;
}

sf::Packet& operator >> (sf::Packet& packet, MsgRand& msg)
{
    return packet >> msg.msgtype >> msg.id >> msg.r;
}

void readMsg(Message m, Msg& msg)
{
    MsgPos* m_pos;
    MsgRand* m_rand;
    MsgReg* m_reg;
    MsgSelect* m_select;
    std::stringstream ss;

    //For debugging purposes
    //char data[256];
    //std::memcpy(data, m.first.getData(), m.first.getDataSize());
    //ss << "ReadMsg Received: ";
    //for (int i = 0; i < m.first.getDataSize(); i++)
    //{
    //    ss << (int) data[i];
    //}
    //ss << std::endl;
    //std::cout << ss.str();

    m.first >> msg.msgtype;
    switch (msg.msgtype)
    {
        case MsgType::Register:
            m_reg = new MsgReg();
            if (m.first >> m_reg->id >> m_reg->username)
            {
                std::memcpy(&msg, m_reg, sizeof(MsgReg));
            }
            else
            {
                ss << "message::readMsg Register message was not understood\n";
            }
            delete m_reg;
            break;
        case MsgType::Select:
            m_select = new MsgSelect();
            if (m.first >> m_select->id >> m_select->builderID)
            {
                std::memcpy(&msg, m_reg, sizeof(MsgSelect));
            }
            else
            {
                ss << "message::readMsg Select message was not understood\n";
            }
            break;
        case MsgType::Build:
        case MsgType::Move:
        case MsgType::Place:
            m_pos = new MsgPos();
            m_pos->msgtype = msg.msgtype;
            if (m.first >> m_pos->id >> m_pos->x >> m_pos->y)
            {
                std::memcpy(&msg, m_pos, sizeof(MsgPos));
            }
            else
            {
                ss << "message::readMsg Place message not understood\n";
            }
            delete m_pos;
            break;
        case MsgType::Rand:
            m_rand = new MsgRand();
            if (m.first >> m_rand->id >> m_rand->r)
            {
                std::memcpy(&msg, m_rand, sizeof(MsgRand));
            }
            else
            {
                ss << "message::readMsg Rand message not understood\n";
            }
            delete m_rand;
            break;
        default:
            ss << "Message type not understood: " << (int)msg.msgtype << std::endl;
            break;
    }
    std::cout << ss.str();
}
