//
//  CBody.h
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CBody__
#define __Third__CBody__

#include <stdio.h>

#include "Define.h"


class CBody {
    
public:
    CBody(Box rect);
    void onLoop();
    
    float velX, velY;
    
    int getX();
    int getY();
    int getW();
    int getH();
    
    Box _rect;
    Box _previousRect;
    
};

#endif /* defined(__Third__CBody__) */
