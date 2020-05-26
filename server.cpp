#include "accepter.hpp"
#include "message.hpp"
#include "queue.hpp"
#include "server.hpp"
#include "util.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <map>
#include <sstream>
#include <thread>

//A print function to output some debugging information about the content of the map
void printClient(const Client& client)
{
    std::stringstream ss;
    ss << "Client: " << client.first << " " << client.second->getRemoteAddress() << std::endl;
    std::cout << ss.str();
}

//runUDP is a function that should be run in a thread. We bind a UDP socket to a port and then, in a loop, when we receive a UDP message, we respond to it with a known answer
void Server::runUDP()
{
    sf::UdpSocket usocket;
    auto runBind = [&usocket] { return usocket.bind(UPORT); };
    net_run(runBind, "UDP bind");
    std::cout << "UDP bound to port: " << usocket.getLocalPort() << std::endl;
    sf::Packet packet;
    sf::IpAddress address;
    unsigned short remote_port;
    while(true)
    {
        packet.clear();
        auto runRecv = [&] {return usocket.receive(packet, address, remote_port); };
        net_run(runRecv, "UDP receive");
        std::cout << "Received UDP message; replying with answer\n";
        packet.clear();
        packet << "Yes I am!";
        usocket.send(packet, address, remote_port);
        std::cout << "UDP message sent back\n";
    }
}

void Server::operator()()
{
    Queue<Message> queue;
    //Map of client: ID, Tcp socket; does not need to be thread-safe anymore
    //So we use a standard library map
    std::map<sf::Uint32, std::shared_ptr<sf::TcpSocket>> clients;
    //Launch our udp receive loop thread
    std::thread(&Server::runUDP, this).detach();
    //Launch our accepter loop thread
    //std::shared_ptr<sf::TcpSocket> s;
    std::thread(Accepter(queue)).detach();
    while(true)
    {
        //This is for debugging purposes: prints all entries in clients
        std::for_each(clients.begin(), clients.end(), printClient);
        Message m = queue.pop(); //m is a pair (packet, socket); blocking pop
        Msg msg;
        readMsg(m, msg); //Sets msg from the message m
        //If the message is a resgister, lets insert the client into our map
        if(msg.msgtype == MsgType::Register)
        {
            //Currently not using the username
            //We are adding a new client with the id and the Tcpsocket
            clients.insert(Client(msg.id, m.second));
            //We should also send to the newly connected client all of the already connected clients. We should just send some more Register messages
            auto send_reg = [&] (const Client& client)
            {
                sf::Packet to_send;
                to_send << MsgReg(client.first, "");
                auto my_send = [&]
                {
                    //We dont send back to OP, but to all others
                    if (client.first != msg.id)
                    {
                        return m.second->send(to_send);
                    }
                    return sf::Socket::Done; //What net_run wants: All okay for id == OP
                };
                net_run(my_send, "send");
            };
            std::for_each(clients.begin(), clients.end(), send_reg);
        }
        //Here we are just checking that the message is a register
        //We could also check the general validity of the message i.e. if the message makes sense with the game
        auto send_to_one = [&] (const Client& client)
        {
            auto my_send = [&]
            {
                //We dont send back to OP, but to all others
                if (client.first != msg.id)
                {
                    return client.second->send(m.first);
                }
                return sf::Socket::Done; //What net_run wants: All okay
            };
            net_run(my_send, "send");
        };
        std::for_each(clients.begin(), clients.end(), send_to_one);
    }
}

Server::Server()
{

}

/*int main()
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

}*/