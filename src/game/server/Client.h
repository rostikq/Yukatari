//
// Created by user on 3/15/2026.
//

#ifndef YUKATARI_CLIENT_H
#define YUKATARI_CLIENT_H
#include "SFML/Network/IpAddress.hpp"


struct Client {
    sf::IpAddress ipAddress;
    unsigned int id;
};


#endif //YUKATARI_CLIENT_H