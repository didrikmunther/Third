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
#include "CGuiText.h"
#include "CText.h"

class CCombatText : public CGuiText {
    
public:
    CCombatText(int x, int y, std::string text, std::string fontKey);
    
    virtual void onLoop();
    virtual void onRender(CWindow* window, CCamera* camera);
    
private:
    CText* _textObj;
    
};

#endif /* defined(__Fifth__CCombatText__) */
