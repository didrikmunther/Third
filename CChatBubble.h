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

class CChatBubble : public CGuiText{

public:
    CChatBubble(std::string text, CEntity* target, std::string fontKey, CAssetManager* assetManager, int type);
    
    void onLoop();
    void onRender(sf::RenderWindow* window, CCamera* camera);
    
private:
    
    sf::Clock clock;
    
    CEntity* target;
    int r, g, b;            // Foreground colors
    int rB, gB, bB;         // Background colors
    int type;
    int livingTime;         // Seconds
    int creationTime;
    
    std::vector<CText> TextVector;
    int widestLine;
    int totalHeight;
    
};

#endif /* defined(__Third__CChatBubble__) */
