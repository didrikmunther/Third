//
//  CGameClient.h
//  Fifth
//
//  Created by Didrik Munther on 17/04/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#ifndef CGameClient_h
#define CGameClient_h

#include <stdio.h>
#include <SDL2_net/SDL_net.h>

#include "CGame.h"


class CGameClient {
    
public:
    CGameClient(CGame* game);
    
    int connect(std::string host, int port);
    
    std::string host;
    int port;
    
    IPaddress serverIP;
    TCPsocket clientSocket;
    
    char buffer[PACKET_SIZE];
    
    SDLNet_SocketSet socketSet;
    
    void onLoop();
    void onEvent(SDL_Event* event);
    
    SDL_Thread* recvThread;
    
    CGame* game;
    
};

#endif /* CGameClient_h */
