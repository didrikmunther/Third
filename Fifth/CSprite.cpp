//
//  CSprite.cpp
//  Third
//
//  Created by Didrik Munther on 24/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CSprite.h"
#include "CSpriteSheet.h"
#include "CWindow.h"
#include "CCamera.h"

#include "NSurface.h"


CSprite::CSprite(CSpriteSheet* spriteSheet, Box rect) :
_spriteSheet(spriteSheet), _source({rect.x, rect.y, rect.w, rect.h}) {  }

void CSprite::onRender(Box destination, bool flip, int angle, CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    NSurface::renderTexture(destination, *getSource(), window->getRenderer(), getSpriteSheet()->getTexture(), flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE, angle);
}