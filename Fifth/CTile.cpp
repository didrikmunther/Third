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

#include "NSurface.h"


CTile::CTile(std::string tileset, int posX, int posY)
    : CEntity(Box{posX * TILE_SIZE, posY * TILE_SIZE, TILE_SIZE, TILE_SIZE}, "")
    , tileset(CAssetManager::getTileset(tileset))
    , tilesetKey(tileset)
    , posX(posX), posY(posY)
    , tileIndex(0)
{
    addProperty(EntityProperty::STATIC);
}

void CTile::onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    NSurface::renderSprite(posX * TILE_SIZE - camera->offsetX(), posY * TILE_SIZE - camera->offsetY(), TILE_SIZE, TILE_SIZE, CAssetManager::getSprite((*tileset)[tileIndex]), window, SDL_RendererFlip::SDL_FLIP_NONE);
}

void CTile::onSerialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc, CInstance* instance) {
    value->PushBack(rapidjson::Value(posX), *alloc);
    value->PushBack(rapidjson::Value(posY), *alloc);
}

void CTile::updateIndex(std::map<int, std::map<int, CTile*>>* _tiles) {
    tileIndex = 0;
    if(tileExist(_tiles, posX, posY-1))
        tileIndex += 1;
    if(tileExist(_tiles, posX+1, posY))
        tileIndex += 2;
    if(tileExist(_tiles, posX, posY+1))
        tileIndex += 4;
    if(tileExist(_tiles, posX-1, posY))
        tileIndex += 8;
}

void CTile::updateAdjecent(std::map<int, std::map<int, CTile*>>* _tiles) {
    if(tileExist(_tiles, posX, posY-1))
        (*_tiles)[posX][posY-1]->updateIndex(_tiles);
    if(tileExist(_tiles, posX+1, posY))
        (*_tiles)[posX+1][posY]->updateIndex(_tiles);
    if(tileExist(_tiles, posX, posY+1))
        (*_tiles)[posX][posY+1]->updateIndex(_tiles);
    if(tileExist(_tiles, posX-1, posY))
        (*_tiles)[posX-1][posY]->updateIndex(_tiles);
}