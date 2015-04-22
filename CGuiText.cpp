//
//  CGuiText.cpp
//  Third
//
//  Created by Didrik Munther on 22/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CGuiText.h"
#include "NSurface.h"

CGuiText::CGuiText(int x, int y, std::string text, TTF_Font* font) :
    x(x), y(y), text(text), font(font), toRemove(false) {
}

void CGuiText::onLoop() {
}
void CGuiText::onRender(SDL_Renderer *renderer, CCamera* camera) {
}