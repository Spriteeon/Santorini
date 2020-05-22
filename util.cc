#include "util.hpp"
#include <iostream>
#include <cstring>

#define BUFF_SIZE 1024

std::string receive(sf::UdpSocket &socket, sf::IpAddress& sender, unsigned short& port)
{
    //We are passing all this by reference
    char data[BUFF_SIZE];
    std::size_t received;
    //Initialising data
    std::memset(data, 0, BUFF_SIZE);
    if(socket.receive(data, BUFF_SIZE, received, sender, port) != sf::Socket::Done)
    {
        std::cerr << "Problems receiving" << std::endl;
        perror("receive");
        exit(0);
    }
    std::cout << "Received: \"" << std::string(data) << "\" from " << sender << " on " << port << std::endl;
    return std::string(data);

}

void send(sf::UdpSocket &socket, std::string msg, sf::IpAddress recipient, unsigned short remote_port)
{
    if (socket.send(msg.c_str(), msg.length() + 1, recipient, remote_port) != sf::Socket::Done)
    {
        std::cerr << "Problems sending" << std::endl;
        perror("send");
        exit(0);
    }
}

void net_run(std::function<sf::Socket::Status(void)> f, std::string error_message)
{
    sf::Socket::Status status = f();
    if (status != sf::Socket::Done)
    {
        std::cerr << "Failed to " << error_message << ": " << status << std::endl;
        perror(error_message.c_str());
        exit(0);
    }
}
