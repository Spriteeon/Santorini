#include <SFML/Network.hpp>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <thread>
#include "util.hpp"

int main()
{

    sf::TcpListener listener;
    sf::UdpSocket usocket;
    if(usocket.bind(UPORT) != sf::Socket::Done)
    {
        std::cerr << "Error binding to UDP port<n";
        return 1;
    }
    std::cout << "UDP bound to port: " << usocket.getLocalPort() << std::endl;
    std::thread([&usocket]{
        sf::Packet packet;
        sf::IpAddress address;
        unsigned short remote_port;
        while(true)
        {
            packet.clear();
            sf::Socket::Status status = usocket.receive(packet, address, remote_port);
            if(status == sf::Socket::Done)
            {
                std::cout << "Received UDP message; replying with answer\n";
                packet.clear();
                packet << "Request granted";
                usocket.send(packet, address, remote_port);
                std::cout << "UDP message sent back\n";
            }
            else
            {
                std::cerr << "Receive status: " << status << std::endl;
            }
            
        }
    }).detach();
    unsigned int local_port = PORT;
    if (listener.listen(local_port) != sf::Socket::Done)
    {
        std::cerr << "TCP Binding error" << std::endl;
        return 1;
    }
    std::cout << "TCP socket bound to port: " << listener.getLocalPort() << std::endl;
    sf::TcpSocket tsocket;
    if(listener.accept(tsocket) != sf::Socket::Done)
    {
        std::cerr << "Accepting problems" << std::endl;
        return 1;
    }
    sf::Packet packet;
    while(true)
    {
        packet.clear();
        if (tsocket.receive(packet) != sf::Socket::Done)
        {
            std::cerr << "Receiving problems" << std::endl;
            return 1;
        }
        std::cout << "Received TCP message\n";
        if (tsocket.send(packet) != sf::Socket::Done)
        {
            std::cerr << "Problems sending" << std::endl;
            return 1;
        }
        std::cout << "Echoed TCP message\n";
    }

}