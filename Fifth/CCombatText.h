//
//  CCombatText.h
//  Fifth
//
//  Created by Didrik Munther on 02/06/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CCombatText__
#define __Fifth__CCombatText__

#include <stdio.h>
#include <SDL2/SDL.h>

#include "CGuiText.h"
#include "NFile.h"


class CText;

class CCombatText : public CGuiText {
    
public:
    CCombatText(int x, int y, Color color, int size, std::string text, std::string fontKey);
    
    virtual void onLoop();
    virtual void onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    
private:
    CText* _textObj;
    
};

#endif /* defined(__Fifth__CCombatText__) */
