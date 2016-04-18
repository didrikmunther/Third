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


class CServerInstance;

class CClient {
    
public:
    CClient(SDLNet_SocketSet* socketSet, TCPsocket socket, int clientId);
    void close(SDLNet_SocketSet* socketSet);
    
    void send(const char* buffer, int len = -1);
    
    int clientId;
    TCPsocket socket;
    
    CServerInstance* instance;
    
    
};

#endif /* defined(__Fifth_Server__CClient__) */
