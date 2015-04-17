//
//  NFile.h
//  Third
//
//  Created by Didrik Munther on 13/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__NFile__
#define __Third__NFile__

#include <stdio.h>
#include <SDL2/SDL.h>
#include "CSpriteSheet.h"

namespace NFile {
    
    // Load functions
    void loadKeymap();
    void testSave(std::string data, std::string key);
    
    CSpriteSheet* loadSpriteSheet();
    
};

#endif /* defined(__Third__NFile__) */
