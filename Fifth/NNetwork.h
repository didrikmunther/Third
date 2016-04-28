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


namespace NNetwork {
    
    void sendToSocket(TCPsocket sock, const char* data, int len = -1);
    
};

#endif /* NNetwork_h */
