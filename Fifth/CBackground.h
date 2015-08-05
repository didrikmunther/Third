//
//  CBackground.h
//  Fifth
//
//  Created by Didrik Munther on 08/07/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CBackground__
#define __Fifth__CBackground__

#include <stdio.h>
#include "CSprite.h"

class CBackground {
    
public:
    CBackground();
    
    int getParallax();
    void setParallax(int parallax);
    
private:
    int parallax;
    
};

#endif /* defined(__Fifth__CBackground__) */
