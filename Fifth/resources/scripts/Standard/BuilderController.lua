-- BuilderController.lua

local BuilderController = class (
    function(self, parent, component)
        self.parent = parent
        self.component = component
                                 
        self.mouseDown = false
        self.rightMouseDown = false
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
        self.isTiling = false
                                 
        self.tileSheet = {}
        self.tiles = Autotable(2)
        self.allTilePositions = {}
    end
)

function BuilderController:onComponentAdd(comp)
    if(comp == "Standard/ChatController") then
        chatController = self.parent:getComponent("Standard/ChatController")
        chatController:registerCommand("build", self)
        chatController:registerCommand("place", self)
        chatController:registerCommand("move", self)
        chatController:registerCommand("remove", self)
        chatController:registerCommand("color", self)
        chatController:registerCommand("tile", self)
    end
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

function BuilderController:tile(commands)
    if(self.isTiling) then
        self.isTiling = false
        do return end
    end

    stem = commands[2]

    for i = 0, 15 do
        self.tileSheet[i] = stem .. i
    end
    
    self.isTiling = true
    
end

function BuilderController:updateTile(posX, posY)
    if(not self.tiles[posX][posY]) then
        do return end
    end

    sum = 0
    if(self.tiles[posX][posY-1]) then sum = sum + 1 end
    if(self.tiles[posX+1][posY]) then sum = sum + 2 end
    if(self.tiles[posX][posY+1]) then sum = sum + 4 end
    if(self.tiles[posX-1][posY]) then sum = sum + 8 end

    spriteKey = self.tileSheet[sum]

    self.tiles[posX][posY].defaultSprite = spriteKey
end

function BuilderController:updateTiles(posX, posY)
    self:updateTile(posX, posY)
    self:updateTile(posX, posY-1)
    self:updateTile(posX+1, posY)
    self:updateTile(posX, posY+1)
    self:updateTile(posX-1, posY)
end

function BuilderController:onCommand(commands)
    if(commands[1] == "build") then     self:build(commands) end
    if(commands[1] == "place") then     self:place(commands) end
    if(commands[1] == "move") then      self:move(commands) end
    if(commands[1] == "remove") then    self:remove(commands) end
    if(commands[1] == "color") then     self:color(commands) end
    if(commands[1] == "tile") then      self:tile(commands) end
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

    if(self.isTiling) then
        if(self.mouseDown) then
            size = 64
            posX = math.floor(self.mX / size)
            posY = math.floor(self.mY / size)
            
            if(not self.tiles[posX][posY]) then
                self.hasSprite = true
                self.spriteKey = "dirt0"
                self:createActiveEntity()
                self.activeEntity.body:setDimension(size, size)
                self.activeEntity.body:setPosition(posX * 64, posY * 64)
                self.activeEntity:addProperty(EntityProperty.STATIC)
                self.tiles[posX][posY] = self.activeEntity
                
                self:updateTiles(posX, posY)

                self.activeEntity = nil
            end
        end
        if(self.rightMouseDown) then
            size = 64
            posX = math.floor(self.mX / size)
            posY = math.floor(self.mY / size)
            
            if(self.tiles[posX][posY]) then
                self.tiles[posX][posY].toRemove = true
            end
            self.tiles[posX][posY] = nil
            self:updateTiles(posX, posY)
        end
    end

end

function BuilderController:onKeyStates(state)
    self.mX, self.mY = self.component:getRelativeMouse()
    self.mouseDown = game:leftMousePressed()
    self.rightMouseDown = game:rightMousePressed()

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