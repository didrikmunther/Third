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
    if(!this->tileset->collide)
        removeProperty(EntityProperty::COLLIDABLE);
}

void CTile::onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    if(!camera->collision(this))
        return;
    
    int x = posX * TILE_SIZE - camera->offsetX();
    int y = posY * TILE_SIZE - camera->offsetY();
    
    if(tileset)
        NSurface::renderSprite(x, y, TILE_SIZE, TILE_SIZE, CAssetManager::getSprite((*tileset)[tileIndex]), window, SDL_RendererFlip::SDL_FLIP_NONE);
    else
        NSurface::renderRect(x, y, TILE_SIZE, TILE_SIZE, window, 255, 0, 255);
}

void CTile::onSerialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc, CInstance* instance) {
    value->PushBack(rapidjson::Value(posX), *alloc);
    value->PushBack(rapidjson::Value(posY), *alloc);
}

void CTile::updateIndex(std::map<int, std::map<int, CTile*>>* _tiles) {
    tileIndex = 0;
    if(tileExist(_tiles, posX, posY-1) && (*_tiles)[posX][posY-1]->tilesetKey == tilesetKey)
        tileIndex += 1;
    if(tileExist(_tiles, posX+1, posY) && (*_tiles)[posX+1][posY]->tilesetKey == tilesetKey)
        tileIndex += 2;
    if(tileExist(_tiles, posX, posY+1) && (*_tiles)[posX][posY+1]->tilesetKey == tilesetKey)
        tileIndex += 4;
    if(tileExist(_tiles, posX-1, posY) && (*_tiles)[posX-1][posY]->tilesetKey == tilesetKey)
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