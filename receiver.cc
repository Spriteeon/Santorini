#include "receiver.hpp"
#include "util.hpp"
#include "message.hpp"
#include <chrono>
#include <cstring>
#include <iostream>
#include <iostream>
#include <sstream>
#include <thread>

#include <SFML/Network.hpp>

Receiver::Receiver(std::shared_ptr<sf::TcpSocket> s,
        Queue<Message> &q):
    socket_(s),
    queue_(q)
{
}

/*void Receiver::recv_loop()
{
    {
        std::stringstream ss;
        ss << socket_->getRemoteAddress() << ":" << socket_->getRemotePort() << std::endl;
        std::cout << ss.str();
    }
    while(true)
    {
        Msg m;
        std::size_t received;
        auto receive = [&] { return socket_->receive(&m, sizeof(m), received); };
        net_run(receive, "receive");
        {
            std::stringstream ss;
            ss << "Received: " << received << " bytes." << std::endl;
            std::cout << ss.str();
        }
        queue_.push(std::move(m));
    }
}*/

void Receiver::recv_loop()
{
    while(true)
    {
        sf::Packet packet;
        auto receive = [&] {return socket_->receive(packet); };
        net_run(receive, "receive");
        {
            std::stringstream ss;
            ss << "\nReceiver::recv_loop Received: " << packet.getDataSize() << " bytes." << std::endl;
            std::cout << ss.str();
        }
        queue_.push(Message(std::move(packet), socket_));
    }
}

