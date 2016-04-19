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
#include <SDL2_net/SDL_net.h>
#include <string>


class CServerInstance;
class CEntity;

class CClient {
    
public:
    CClient(SDLNet_SocketSet* socketSet, TCPsocket socket, int clientId);
    void initEntities();
    void close(SDLNet_SocketSet* socketSet);
    
    void send(std::string* data);
    
    void onEvent(std::string event);
    
    int clientId;
    TCPsocket socket;
    std::string rest;
    
    CServerInstance* instance;
    
    CEntity* player;
    CEntity* controller;
    
};

#endif /* defined(__Fifth_Server__CClient__) */
