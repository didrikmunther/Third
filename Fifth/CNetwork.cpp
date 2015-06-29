//
//  CNetwork.cpp
//  Fifth
//
//  Created by Didrik Munther on 13/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CNetwork.h"

CNetwork::CNetwork() : _isRunning(true), clientId(0) {
}

int CNetwork::onInit(CGame *game) {
    
    if(_tcpListener.listen(1337) != sf::Socket::Done) {
        return -1;
    } //port
    
    sf::Thread connectionsThread(&CNetwork::_connectionsLoop, this);
    connectionsThread.launch();
    
    return 0;
}

void CNetwork::_connectionsLoop() {
    while(_isRunning) {
        sf::TcpSocket* tcpClient;
        if(_tcpListener.accept(*tcpClient) != sf::Socket::Done) {
            /* tough shit */
        }
        CClient* client = new CClient(tcpClient);
        _clients[std::to_string(clientId++)] = client;
        
    }
}

void CNetwork::onUpdate() {
    
}

