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


class CCamera;

namespace NMouse {
    
    int absoluteMouseX();
    int absoluteMouseY();
    
    int relativeMouseX(CCamera* camera);
    int relativeMouseY(CCamera* camera);
    
    bool leftMouseButtonPressed();
    bool rightMouseButtonPressed();
    
};

#endif /* defined(__Third__NMouse__) */
