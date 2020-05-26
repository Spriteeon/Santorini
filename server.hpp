#include <SFML/Network.hpp>
#include <memory>

//A client is a pair of ID and TcpSocket
//What is relevent as a key for a client?
//Here we use an ID
typedef std::pair<sf::Uint32, std::shared_ptr<sf::TcpSocket>> Client;

class Server
{
    public:
        Server();
        void operator()();
    private:
        void runUDP();
};