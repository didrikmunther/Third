//
//  CClient.cpp
//  Fifth-Server
//
//  Created by Didrik Munther on 21/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CClient.h"
#include "NNetwork.h"


CClient::CClient(SDLNet_SocketSet* socketSet, TCPsocket socket, int clientId)
    : socket(socket)
    , clientId(clientId)
{
    SDLNet_TCP_AddSocket(*socketSet, socket);
}

void CClient::close(SDLNet_SocketSet* socketSet) {
    SDLNet_TCP_DelSocket(*socketSet, socket);
    SDLNet_TCP_Close(socket);
}

void CClient::send(const char* buffer, int len /* = -1 */) {
    NNetwork::sendToSocket(socket, buffer, len);
}