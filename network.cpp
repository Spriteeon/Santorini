#include "network.hpp"
#include "receiver.hpp"
#include <iostream>
#include <SFML/Network.hpp>
#include <thread>

Network::Network(Queue<Message>& q):
    queue(q)
{
    //UDP message to find server
    sf::UdpSocket udpSocket;
    sf::Packet packet;
    packet << "Are you a server?";
    unsigned short port = UPORT;
    std::cout << "Sending UDP message discovery: " << port << std::endl;
    auto sendUDP = [&] {return udpSocket.send(packet, sf::IpAddress::Broadcast, UPORT); };
    net_run(sendUDP, "UDP Send");
    std::cout << "Sent UDP message\n";

    //Receive UDP message from server:
    packet.clear();
    sf::IpAddress address;
    auto recvUDP = [&] {return udpSocket.receive(packet, address, port); };
    net_run(recvUDP, "UDP Receive");
    std::cout << "Received UDP message back from server\n";

    //Connect to the Address
    socket = std::make_shared<sf::TcpSocket>();
    auto connect = [&] {return socket->connect(address, PORT); };
    net_run(connect, "connect");
    std::cout << "Client::main Connected\n";

    //Register with the server first
    MsgReg m_reg;
    //Let's give the player a random ID
    m_reg.id = rand() % 16384;
    //Let's give the player a name
    m_reg.username = "Batman";
    sf::Packet m_reg_packet;
    m_reg_packet << m_reg;
    auto send1 = [&] {return socket->send(m_reg_packet); };
    net_run(send1, "send");

    //This should not do anything with an echo server
    //We will just get our message back
    //However a real server will need this
    std::cout << "Sent register\n";

    //Start Receiver thread
    //std::shared_ptr<Receiver> r = std::make_shared<Receiver>(socket, queue);
    //std::thread(&Receiver::recv_loop, r).detach();
    Receiver r(socket, queue);
    std::thread(&Receiver::recv_loop, std::move(r)).detach();

}

Network::~Network()
{

}

void Network::send(const MsgReg& msg)
{
    sf::Packet packet;
    packet << msg;
    socket->send(packet);
}

void Network::send(const MsgSelect& msg)
{
    sf::Packet packet;
    packet << msg;
    socket->send(packet);
}

void Network::send(const MsgRand& msg)
{
    sf::Packet packet;
    packet << msg;
    socket->send(packet);
}

void Network::send(const MsgPos& msg)
{
    sf::Packet packet;
    packet << msg;
    socket->send(packet);
}
