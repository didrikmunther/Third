//
//  CTile.h
//  Third
//
//  Created by Didrik Munther on 02/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CTile__
#define __Third__CTile__

#include <stdio.h>
#include <string>

#include "CSprite.h"


class CWindow;
class CCamera;
enum RenderFlags;

class CTile {
    
public:
    CTile(std::string spriteKey); // Tile sets its sprite instantly instead of having a sprite key saved
    
    void onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    
private:
    CSprite* sprite;
    
};

#endif /* defined(__Third__CTile__) */
