//
// Created by user on 3/14/2026.
//

#ifndef YUKATARI_NETWORKMANAGER_H
#define YUKATARI_NETWORKMANAGER_H
#include "EventManager.h"
#include "IObserver.h"
#include "SFML/Network/IpAddress.hpp"
#include "SFML/Network/UdpSocket.hpp"


class NetworkManager : public IObserver, public EventManager {
private:
    std::optional<sf::IpAddress> m_serverIp;
    sf::UdpSocket m_socket;
    unsigned short m_port = 0;

    bool m_isConnected = false;

public:
    NetworkManager();
    ~NetworkManager() = default;

    void connect(sf::IpAddress ipAddress, unsigned int port);
    void send(sf::Packet& packet);
    void disconnect();

    void pollPackets();

    void onEvent(IEvent &event) override;

};


#endif //YUKATARI_NETWORKMANAGER_H