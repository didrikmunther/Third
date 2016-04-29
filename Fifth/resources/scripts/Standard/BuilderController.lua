-- BuilderController.lua

local BuilderController = class (
    function(self, parent, component)
        self.parent = parent
        self.component = component
                                 
        self.mouseDown = false
        self.initMX = 0
        self.initMY = 0
        self.mX = 0
        self.mY = 0
        
        self.hasSprite = false
        self.spriteKey = ""
        self.r = 255
        self.g = 0
        self.b = 0
        self.a = 255
                       
        self.activeEntity = nil
        self.editValues = false
                                 
        self.isBuilding = false
        self.isMoving = false
    end
)

function BuilderController:onInit()
    chatController = self.parent:getComponent("Standard/ChatController")
    chatController:registerCommand("build", self)
    chatController:registerCommand("place", self)
    chatController:registerCommand("move", self)
    chatController:registerCommand("remove", self)
    chatController:registerCommand("color", self)
end

function BuilderController:createActiveEntity()
    if(self.hasSprite) then
        self.activeEntity = self.parent.entityManager:createSpriteEntity(Box(self.mX, self.mY, 0, 0), self.spriteKey)
    else
        self.activeEntity = self.parent.entityManager:createColoredEntity(Box(self.mX, self.mY, 0, 0), Color(self.r, self.g, self.b, self.a))
    end
       
    self.parent.entityManager:addEntity(self.activeEntity, "")
end

function BuilderController:build(commands)
    self.isBuilding = true
end

function BuilderController:place(commands)
    if(commands[2] and commands[3]) then
        self.isMoving = true
        self:createActiveEntity()
        self.activeEntity.body:setDimension(tonumber(commands[2]), tonumber(commands[3]))
        
        flagsToToggle = BitOR(EntityProperty.COLLIDABLE, EntityProperty.GRAVITY_AFFECT)
        self.activeEntity:removeProperty(flagsToToggle)
    end
end

function BuilderController:move(commands)
    self.activeEntity = self.parent.entityManager:getEntityAtCoordinate(self.mX, self.mY, nil)
    if(not self.activeEntity) then do return end end

    self.activeEntity:removeProperty(BitOR(EntityProperty.COLLIDABLE, EntityProperty.GRAVITY_AFFECT))
    self.isMoving = true
end

function BuilderController:remove(commands)
    self.activeEntity = self.parent.entityManager:getEntityAtCoordinate(self.mX, self.mY, nil)
    if(not self.activeEntity) then do return end end

    self.activeEntity.isDead = true
    self.activeEntity = nil
end

function BuilderController:color(commands)
    if(commands[3] == nil) then
        self.spriteKey = commands[2]
        self.hasSprite = true
    else
        self.r = tonumber(commands[2])
        self.g = tonumber(commands[3])
        self.b = tonumber(commands[4])
        if(commands[5]) then
            self.a = tonumber(commands[5])
        end
        self.hasSprite = false
    end
end

function BuilderController:onCommand(commands)
    if(commands[1] == "build") then     self:build(commands) end
    if(commands[1] == "place") then     self:place(commands) end
    if(commands[1] == "move") then      self:move(commands) end
    if(commands[1] == "remove") then    self:remove(commands) end
    if(commands[1] == "color") then     self:color(commands) end
end

function BuilderController:onLoop()

    if(self.isBuilding) then
        if(self.mouseDown and self.activeEntity == nil) then
            self.initMX = self.mX
            self.initMY = self.mY
            self:createActiveEntity()
            
            flagsToToggle = BitOR(EntityProperty.COLLIDABLE, EntityProperty.GRAVITY_AFFECT)
            self.activeEntity:removeProperty(flagsToToggle)
        end

        if(not self.mouseDown and self.activeEntity ~= nil) then
            flagsToToggle = EntityProperty.COLLIDABLE
            self.activeEntity:addProperty(flagsToToggle)
            
            self.activeEntity = nil
            self.editValues = true
            self.isBuilding = false
            --self.component.instance.game.ignoreEvents = true
        end

        if(self.activeEntity ~= nil) then
            body = self.activeEntity.body
            
            dX = self.mX - self.initMX
            dY = self.mY - self.initMY
            self.activeEntity.body:setDimension(math.abs(dX), math.abs(dY))
            
            if(dX < 0) then
                body:setPosition(self.mX, body.box.y)
            end
            if(dY < 0) then
                body:setPosition(body.box.x, self.mY)
            end
        end
    end

    if(self.isMoving) then
        if(self.activeEntity ~= nil) then
            self.activeEntity.body:setPosition(self.mX, self.mY)
        else
            self.isMoving = false
        end
        
        if(self.mouseDown) then
            self.isMoving = false
            
            self.activeEntity:addProperty(EntityProperty.COLLIDABLE)
            self.activeEntity = nil
        end
    end

end

function BuilderController:onKeyStates(state)
    self.mX, self.mY = self.component:getRelativeMouse()
    self.mouseDown = game:leftMousePressed()

end

function BuilderController:onEvent(key, keyDown)

    if(keyDown) then
        if(self.editValues) then
            
        end
    end

end

function create(parent, component)
    return BuilderController(parent, component)
end