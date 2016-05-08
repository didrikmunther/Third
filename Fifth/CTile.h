//
//  CTile.h
//  Third
//
//  Created by Didrik Munther on 02/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CTile__
#define __Third__CTile__

#include <stdio.h>
#include <string>
#include <map>

#include "Define.h"
#include "CEntity.h"


class CWindow;
class CCamera;

struct Tileset {
    std::string spriteKeys[16];
    bool collide = true;
    bool isBackground = true;
    
    std::string operator[](int i) {
        if(i <= 15 && i >= 0)
            return spriteKeys[i];
        else
            return "";
    }
};

class CTile : public CEntity {
    
public:
    CTile(std::string tileset, int posX, int posY);
    
    void onLoop();
    void onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    void onSerialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc, CInstance* instance);
    
    void updateIndex(std::map<int, std::map<int, CTile*>>* _tiles);
    void updateAdjecent(std::map<int, std::map<int, CTile*>>* _tiles);
    
    int posX, posY;
    
    static inline bool tileExist(std::map<int, std::map<int, CTile*>>* tiles, int posX, int posY) {
        return tiles->find(posX) != tiles->end() && (*tiles)[posX].find(posY) != (*tiles)[posX].end();
    }
    bool tileBorders(std::map<int, std::map<int, CTile*>>* tiles, int posX, int posY) {
        if(invalid) return true;
        if(tileset->collide || TILE_BG_BORDERS)
            return tileExist(tiles, posX, posY) && (*tiles)[posX][posY]->tilesetKey == tilesetKey;
        else
            return tileExist(tiles, posX, posY);
    }
    
    std::string tilesetKey;
    Tileset* tileset;
    
    bool invalid;
    
private:
    int tileIndex;
    
};

#endif /* defined(__Third__CTile__) */
