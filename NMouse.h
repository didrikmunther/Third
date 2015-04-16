//
//  NMouse.h
//  Third
//
//  Created by Didrik Munther on 16/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__NMouse__
#define __Third__NMouse__

#include <stdio.h>
#include <SDL2/SDL.h>
#include "CCamera.h"

namespace NMouse {
    
    int absoluteMouseX();
    int absoluteMouseY();
    
    int relativeMouseX(CCamera* camera);
    int relativeMouseY(CCamera* camera);
    
};

#endif /* defined(__Third__NMouse__) */
