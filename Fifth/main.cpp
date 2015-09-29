//
//  main.cpp
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CGame.h"

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include <iostream>

using namespace rapidjson;


int main(int argc, const char * argv[]){
    
    CGame game;
    
    return game.onExecute();
}