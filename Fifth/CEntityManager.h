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
#include "CEntity.h"
#include "CParticleEmitter.h"
#include "CSprite.h"
#include <map>
#include <string>
#include "CAssetManager.h"
#include "CGuiText.h"
#include <memory>

class CCamera;
class CInstance;

class CEntityManager {
    
public:
    CEntityManager();
    
    std::shared_ptr<CEntity> addEntity(Box rect, SDL_Color color, std::string name = "");
    std::shared_ptr<CEntity> addEntity(Box rect, std::string spriteKey, std::string name = "");
    std::shared_ptr<CEntity> getEntity(std::string name);
    std::shared_ptr<CEntity> getEntityAtCoordinate(int x, int y);
    std::string getNameOfEntity(std::shared_ptr<CEntity> entity);
    std::string addEntity(std::shared_ptr<CEntity> entity, std::string name = "");
    void addParticle(Box rect, SDL_Color color, int livingTime);
    void addParticle(std::shared_ptr<CEntity> entity);
    void addGuiText(std::shared_ptr<CGuiText> guiText);
    
    void onRender(CWindow* window, CCamera* camera);
    void onLoop(CInstance* instance);
    void onCleanup();
    
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
    std::map<std::string, std::shared_ptr<CEntity>> _EntityVector;
    std::vector<std::shared_ptr<CEntity>> _ParticleVector;
    std::vector<std::shared_ptr<CGuiText>> _GuiTextVector;
    std::map<std::string, std::shared_ptr<CEntity>> _DeadEntitiesVector;
    
    std::map <int, std::map <int, std::vector<CEntity*>>> _CollisionVector;
    int _gridSize;
    
};

#endif /* defined(__Third__CEntityManager__) */
