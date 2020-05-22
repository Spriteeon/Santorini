#include <SFML/Network.hpp>
#include <functional>
#include <memory>

#define PORT 4300

#define UPORT 4400

using Message = std::pair<sf::Packet, std::shared_ptr<sf::TcpSocket>>;

void net_run(std::function<sf::Socket::Status(void)> f, std::string error_message);

