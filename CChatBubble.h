//
//  CChatBubble.h
//  Third
//
//  Created by Didrik Munther on 22/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CChatBubble__
#define __Third__CChatBubble__

#include <stdio.h>
#include "CGuiText.h"
#include "CEntity.h"
#include "CText.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>

class CChatBubble : public CGuiText{

public:
    CChatBubble(std::string text, CEntity* target, TTF_Font* font, int type);
    
    void onLoop();
    void onRender(SDL_Renderer *renderer, CCamera* camera);
    
private:
    CEntity* target;
    int type;
    int dissapearTime;
    
    std::vector<CText> TextVector;
    
};

#endif /* defined(__Third__CChatBubble__) */
