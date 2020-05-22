#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include <SFML/Network.hpp>
#include "queue.hpp"
#include "message.hpp"

class Receiver {
    public:
        Receiver(std::shared_ptr<sf::TcpSocket> s, Queue<Message>& queue);
        void recv_loop();
    private:
        std::shared_ptr<sf::TcpSocket> socket_;
        Queue<Message> &queue_;
};

#endif
