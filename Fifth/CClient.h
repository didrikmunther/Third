//
//  CClient.h
//  Fifth-Server
//
//  Created by Didrik Munther on 21/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth_Server__CClient__
#define __Fifth_Server__CClient__

#include <stdio.h>
#include <SFML/Network.hpp>

class CClient {
    
public:
    CClient(sf::TcpSocket& tcpSocket);
    
    sf::TcpSocket tcpSocket;
    
};

#endif /* defined(__Fifth_Server__CClient__) */
