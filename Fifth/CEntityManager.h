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
#include "CParticle.h"
#include "CParticleEmitter.h"
#include "CSprite.h"
#include <map>
#include <string>
#include "CAssetManager.h"
#include "CGuiText.h"

class CCamera;

class CEntityManager {
    
public:
    CEntityManager();
    
    CEntity* addEntity(Box rect, SDL_Color color, std::string name = "");
    CEntity* addEntity(Box rect, std::string spriteKey, std::string name = "");
    CEntity* getEntity(std::string name);
    CEntity* getEntityAtCoordinate(int x, int y);
    std::string getNameOfEntity(CEntity* entity);
    std::string addEntity(CEntity* entity, std::string name = "");
    void addParticle(Box rect, SDL_Color color, int livingTime);
    void addParticle(CParticle* particle);
//    void addParticleEmitter(SDL_Rect rect, SDL_Color color, int type, int amount, int frequency, int livingTime, int particleLivingTime, ParticleVelocity velocity);
    void addGuiText(CGuiText* guiText);
    
    void onRender(CWindow* window, CCamera* camera);
    void onLoop();
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
    std::map<std::string, CEntity*> _EntityVector;
    std::vector<CParticle*> _ParticleVector;
//    std::vector<CParticleEmitter*> _ParticleEmitterVector;
    std::vector<CGuiText*> _GuiTextVector;
    std::map<std::string, CEntity*> _DeadEntitiesVector;
    
    std::map <int, std::map <int, std::vector<CEntity*>>> _CollisionVector;
    int _gridSize;
    
};

#endif /* defined(__Third__CEntityManager__) */
