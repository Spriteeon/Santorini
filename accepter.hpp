#ifndef ACCEPTER_HPP
#define ACCEPTER_HPP

#include "queue.hpp"
#include "message.hpp"
#include <SFML/Network.hpp>

class Accepter {
    private:
        Queue<Message>& queue_;

    public:
        Accepter(Queue<Message>& q);
        ~Accepter();
        void operator()();
};

#endif