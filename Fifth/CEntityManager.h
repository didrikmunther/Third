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


class CEntity;
class CParticle;
class CSprite;
class CGuiText;
class CBackground;
class CCamera;
class CWindow;

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
    void addGuiText(CGuiText* guiText);
    void addBackground(std::string name, CBackground* background);
    
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
    std::vector<CGuiText*> _GuiTextVector;
    std::map<std::string, CEntity*> _DeadEntitiesVector;
    
    std::map<std::string, CBackground*> _BackgroundVector;
    
    int _gridSize;
    
};

#endif /* defined(__Third__CEntityManager__) */
