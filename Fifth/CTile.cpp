//
//  CTile.cpp
//  Third
//
//  Created by Didrik Munther on 02/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CTile.h"
#include "CAssetManager.h"
#include "CWindow.h"
#include "CCamera.h"


CTile::CTile(std::string spriteKey)
    : sprite(CAssetManager::getSprite(spriteKey))
{  }

void CTile::onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    
}