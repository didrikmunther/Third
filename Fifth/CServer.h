//
//  CServer.h
//  Fifth-Server
//
//  Created by Didrik Munther on 21/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth_Server__CServer__
#define __Fifth_Server__CServer__

#include <stdio.h>
#include <map>
#include <SDL2_net/SDL_net.h>

#include "CServerInstance.h"


class CServer {
    
public:
    CServer(float tickrate, int maxClients = 10);
    
    int onExecute();
    int _onInit();
    int _onRestart();
    
    void _onLoop();
    int _onCleanup();
    
    bool _isRunning;
    
    // Timer
    
    float _lastTime;
    float _timer;
    const float _ns;
    float _delta;
    int _frames;
    int _updates;
    
    // Misc
    
    int currClientId;
    
    std::map<int, CClient*> clients;
    std::map<std::string, CServerInstance*> _instances;

    SDL_Thread* _inputThread;
    SDL_Thread* _broadcastThread;
    
    const unsigned short maxClients;
    const unsigned short maxSockets;
    unsigned short clientCount;
    
    SDLNet_SocketSet socketSet;
    char buffer[PACKET_SIZE];
    
    IPaddress serverIP;
    TCPsocket serverSocket;
    
    void broadcast(CClient* except, const char* data, int len = -1) {
        for(auto& client: clients) {
            if(client.second != except)
                client.second->send(data, len);
        }
    }
};

#endif /* defined(__Fifth_Server__CServer__) */
