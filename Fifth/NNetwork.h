//
//  NNetwork.h
//  Fifth
//
//  Created by Didrik Munther on 17/04/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#ifndef NNetwork_h
#define NNetwork_h

#include <stdio.h>
#include <SDL2_net/SDL_net.h>
#include <string>


namespace NNetwork {
    
    int sendToSocket(TCPsocket sock, const char* data, int len = -1);
    
    void sendPacket(TCPsocket sock, std::string* data);
    std::pair<std::string, std::string> recvPacket(TCPsocket sock, std::string rest);
    
};

#endif /* NNetwork_h */
