//
//  CNetwork.h
//  Fifth
//
//  Created by Didrik Munther on 13/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CNetwork__
#define __Fifth__CNetwork__

#include <stdio.h>
#include <SFML/Network.hpp>
#include <map>
#include <string>
#include "CClient.h"

class CGame;

class CNetwork {
    
public:
    CNetwork();
    
    int connect(sf::IpAddress adress, unsigned short port);
    void disconnect();
    
    void onUpdate(CGame* game);
    
private:
    sf::UdpSocket _udpSocket;
    sf::TcpSocket _tcpSocket;
    
    std::map<std::string, CClient> _clients;
    
    
    
};

#endif /* defined(__Fifth__CNetwork__) */
