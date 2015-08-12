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
#include "CText.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class CCamera;
class CEntity;
class CText;

class CChatBubble : public CGuiText {

public:
    CChatBubble(std::string text, CEntity* target, std::string fontKey, ChatBubbleType type);
    
    void onLoop();
    void onRender(CWindow* window, CCamera* camera);
    
private:
    
    CEntity* _target;
    int _r, _g, _b;            // Foreground colors
    int _rB, _gB, _bB;         // Background colors
    ChatBubbleType _type;
    
    std::vector<CText> _TextVector;
    int _widestLine;
    int _totalHeight;
    
};

#endif /* defined(__Third__CChatBubble__) */
