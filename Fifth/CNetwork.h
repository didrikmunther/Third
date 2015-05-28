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
    
    int onInit(CGame* game);
    void onUpdate(CGame* game);
    
    void onCleanup();
    
private:
    bool _isRunning;
    
    sf::TcpListener _tcpListener;
    
    std::map<std::string, CClient> _clients;
    
    void _connectionsLoop();
    void _clientLoop();
    
    
    
};

#endif /* defined(__Fifth__CNetwork__) */
