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

class CGame;

class CNetwork {
    
public:
    CNetwork();
    
    bool isConnected();
    int connect(sf::IpAddress adress, unsigned short port);
    void disconnect();
    
    void onUpdate(CGame* game);
    
private:
    sf::UdpSocket _udpSocket;
    sf::TcpSocket _tcpSocket;
    
    
    
};

#endif /* defined(__Fifth__CNetwork__) */
