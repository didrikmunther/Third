//
//  CFile.h
//  Third
//
//  Created by Didrik Munther on 13/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CFile__
#define __Third__CFile__

#include <stdio.h>
#include <SDL2/SDL.h>
#include <vector>

class CFile {
    
public:
    
    // Load functions
    void loadKeymap();
    void testSave(std::string data, std::string key);
    
};

#endif /* defined(__Third__CFile__) */
