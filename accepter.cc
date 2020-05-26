#include "accepter.hpp"
#include "receiver.hpp"
#include "util.hpp"
#include "message.hpp"
#include <iostream>
#include <sstream>
#include <thread>

Accepter::Accepter(Queue<Message>& q):
    queue_(q)
{

}

Accepter::~Accepter()
{
}

void Accepter::operator()()
{
    sf::TcpListener listener;
    auto listen = [&] {return listener.listen(PORT); };
    net_run(listen, "listen");
    std::cout << "Bound to port\n";
    while (true)
    {
        std::shared_ptr<sf::TcpSocket> socket = std::make_shared<sf::TcpSocket>();
        auto accept = [&] {return listener.accept(*socket); };
        net_run(accept, "accept");
        //sockets_.push(socket);
        std::stringstream ss;
        ss << "Accepted a connection from: "
            << socket->getRemoteAddress()
            << ":"
            << socket->getRemotePort()
            << std::endl;
        std::cout << ss.str();
        std::shared_ptr<Receiver> receiver = std::make_shared<Receiver>(socket, queue_);
        //Receiver receiver{socket, queue_};
        // TODO launch a thread to receive with the receiver
        //std::thread(receiver).detach();
        std::thread {&Receiver::recv_loop, receiver}.detach();
        //Receiver(socket, queue_)
    }
}