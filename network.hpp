#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "message.hpp"
#include "queue.hpp"
#include "util.hpp"
#include <SFML/Network.hpp>

class Network
{
    public:
        Network(Queue<Message>& q);
        ~Network();
        
        void reg(const sf::Uint32 id);
        void send(const MsgReg& msg);
        void send(const MsgSelect& msg);
        void send(const MsgRand& msg);
        void send(const MsgPos& msg);

    private:
        Queue<Message>& queue;
        std::shared_ptr<sf::TcpSocket> socket;

};

#endif