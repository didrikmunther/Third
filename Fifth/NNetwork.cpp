//
//  NNetwork.cpp
//  Fifth
//
//  Created by Didrik Munther on 17/04/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#include "NNetwork.h"


void NNetwork::sendToSocket(TCPsocket sock, const char* data, int len /* = -1 */) {
    SDLNet_TCP_Send(sock, (void*)data, len == -1 ? (int)strlen(data) + 1 : len);
}