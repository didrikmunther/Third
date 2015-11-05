//
//  CEntityManager.h
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CEntityManager__
#define __Third__CEntityManager__

#include <stdio.h>
#include <memory>
#include <vector>
#include <map>
#include <string>

#include "Define.h"
#include "NSurface.h"


class CEntity;
class CSprite;
class CGuiText;
class CBackground;
class CCamera;
class CWindow;
class CInstance;
class lua_State;

class CEntityManager {
    
public:
    CEntityManager();
    
    CEntity* getEntity(std::string name);
    CEntity* getEntityAtCoordinate(int x, int y);
    std::string getNameOfEntity(CEntity* entity);
    std::string addEntity(CEntity* entity, std::string name = "");
    void addParticle(CEntity* particle);
    void addGuiText(CGuiText* guiText);
    void addBackground(std::string name, CBackground* background);
    
    void onRender(CWindow* window, CCamera* camera);
    void onLoop(CInstance* instance);
    void onCleanup();
    
    CEntity* createEntity(Box box, Color color);
    CEntity* createEntity(Box box, std::string spriteKey);
    
    int pushEntities(lua_State* L);
    
    // Temp
    void splitEntityToParticles(CEntity* target);
    
    void entityCleanup();
    void particleEmitterCleanup();
    void particleCleanup();
    void guiTextCleanup();
    
    int entityID;
    
    int renderFlags;
    void addRenderFlag(RenderFlags renderFlag);
    void removeRenderFlag(RenderFlags renderFlag);
    void toggleRenderFlag(RenderFlags renderFlag);
    
private:
    std::map<std::string, CEntity*> _entities;
    std::vector<CEntity*> _particles;
    std::vector<CGuiText*> _guiTextElements;
    std::map<std::string, CEntity*> _deadEntities;
    std::map<std::string, CBackground*> _backgrounds;
    
    int _gridSize;
    
};

#endif /* defined(__Third__CEntityManager__) */
