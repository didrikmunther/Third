//
//  main.cpp
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CGame.h"
#include "CServer.h"

int client(std::string host) {
    CGame game;
    return game.onExecute(host);
}

int server(int tickrate) {
    CServer server(tickrate);
    return server.onExecute();
}

int main(int argc, const char * argv[]){
    
    int returned = -1;
    
    if(argc >= 3) {
        std::string argv1 = argv[1];
        std::string argv2 = argv[2];
        if(argv1 == "+server") {
            server(stoi(argv2));
        } else if(argv1 == "+client")
            client(argv2);
        else
//            server(60);
            client(argv2);
    } else {
        client("localhost");
    }
    
    printf("Good bye!");
    return returned;
}