-- Movable.lua

local Movable = class (
    function(self, parent, component)
        self.parent = parent
        self.component = component
        self.body = parent.body

        self.jumpPower = 15.0
        self.accelerationX = 1.5
        self.accelerationY = 100.0
        self.stoppingAccelerationX = 1.5
        self.stoppingAccelerationY = 100.0

        self.hasWalkedX = false
        self.hasWalkedY = false

        self.isFlying = false

        self.WALKING_MOVEMENT = 0
        self.SNEAKING_MOVEMENT = 1
        self.RUNNING_MOVEMENT = 2

        self.movementState = self.WALKING_MOVEMENT

        self.movementSpeeds = {}
        self.movementSpeeds[self.WALKING_MOVEMENT] = 10.0
        self.movementSpeeds[self.SNEAKING_MOVEMENT] = self.movementSpeeds[self.WALKING_MOVEMENT] / 2.0
        self.movementSpeeds[self.RUNNING_MOVEMENT] = self.movementSpeeds[self.WALKING_MOVEMENT] * 2.0

    end
)

function Movable:onLoop()

    local body = self.body

	if(not self.hasWalkedX) then
        if(body.velX < 0) then
            body.velX = body.velX + self.stoppingAccelerationX
            if(body.velX >= 0) then
                body.velX = 0.0
            end
        else
            body.velX = body.velX - self.stoppingAccelerationX
            if(body.velX <= 0) then
                body.velX = 0.0
            end
        end
    end
    
    if(not self.hasWalkedY) then
        if(self.isFlying) then
            if(body.velY < 0) then
                body.velY = body.velY + self.stoppingAccelerationY
                if(body.velY >= 0) then
                    body.velY = 0.0
                end
            else
                body.velY = body.velY - self.stoppingAccelerationY
                if(body.velY <= 0) then
                    body.velY = 0.0
                end
            end
        end
    end
    
    self.hasWalkedX = false
    self.hasWalkedY = false

end

function Movable:onEvent(key, keyDown)
    if(keyDown) then
        if(key == KeyCode._SLASH) then
            living = self.parent:getComponent("Standard/Living")
            if(living ~= nil) then
                living:heal(10, self.parent)
            end
        end

        if(key == KeyCode._1) then
            self:toggleNoClip()
        end

        if(key == KeyCode._2) then
            self.parent:toggleProperty(EntityProperty.HIDDEN)
        end

        if(key == toKeyCode(ScanCode._LSHIFT)) then
            self.movementState = self.SNEAKING_MOVEMENT
        end

        if(key == toKeyCode(ScanCode._LALT)) then
            self.movementState = self.RUNNING_MOVEMENT
        end

    else
        if(key == toKeyCode(ScanCode._LALT) or key == toKeyCode(ScanCode._LSHIFT)) then
            self.movementState = self.WALKING_MOVEMENT
        end
    end
end

function Movable:onKeyStates(state)
    if(state:hasState(ScanCode._D)) then
        self:goRight()
    end
    if(state:hasState(ScanCode._A)) then
        self:goLeft()
    end
    if(state:hasState(ScanCode._W) or state:hasState(ScanCode._SPACE)) then
        self:goUp()
    end
    if(state:hasState(ScanCode._S)) then
        self:goDown()
    end

    if(game:leftMousePressed()) then
        box = self.parent.body.box
        thisX = box.x
        thisY = box.y

        bullet = self.parent.entityManager:createColoredEntity(Box(thisX, thisY - 100, 5, 5), Color(200, 50, 50, 255))
        self.parent.entityManager:addParticle(bullet)
        script = game.getScript("Standard/Projectile")
        bullet:addComponent(self.component.instance, script)
        bullet:getComponent("Standard/Projectile"):onDeserialize('{"owner":"' .. self.parent.entityManager:getNameOfEntity(self.parent) .. '"}')

        tBody = self.parent.body
        bBody = bullet:getComponent("Standard/Projectile").parent.body
        bBody.velX = bBody.velX + tBody.velX
        bBody.velY = bBody.velY + tBody.velY

    end
end

function Movable:onDeserialize(value)
    decoded = json.decode(value)

    self.movementSpeeds[self.WALKING_MOVEMENT] = getVal(self.movementSpeeds[self.WALKING_MOVEMENT], decoded.walking_movement_speed)
    self.jumpPower = getVal(self.jumpPower, decoded.jumpPower)
    self.movementState = getVal(self.movementState, decoded.movementState)
    
end

function Movable:goRight()
    body = self.parent.body

	body.velX = body.velX + self.accelerationX;
    
    if(body.velX > self.movementSpeeds[self.movementState]) then
        body.velX = self.movementSpeeds[self.movementState]
    end
    
    self.hasWalkedX = true
end

function Movable:goLeft()
    body = self.parent.body

	body.velX = body.velX - self.accelerationX
    
    if(body.velX < -self.movementSpeeds[self.movementState]) then
        body.velX = -self.movementSpeeds[self.movementState]
    end
    
    self.hasWalkedX = true;
end

function Movable:goUp()
    body = self.parent.body

	if(self.isFlying) then
        body.velY = body.velY - self.accelerationY
        
        if(body.velY < -self.movementSpeeds[self.movementState]) then
            body.velY = -self.movementSpeeds[self.movementState]
        end
    else
        self:jump()
    end
    
    self.hasWalkedY = true
end

function Movable:goDown()
    body = self.parent.body

	if(self.isFlying) then
        body.velY = body.velY + self.accelerationY
        
        if(body.velY > self.movementSpeeds[self.movementState]) then
            body.velY = self.movementSpeeds[self.movementState]
        end
    end
    
    self.hasWalkedY = true
end

function Movable:jump()
    body = self.parent.body

	if(not self.parent.collisionSides.bottom) then
        do return end
    end
    
    body.velY = body.velY - self.accelerationY
    if(body.velY < -self.jumpPower) then
        body.velY = -self.jumpPower
    end
end

function Movable:toggleNoClip()

    flagsToToggle = BitOR(EntityProperty.COLLIDABLE, EntityProperty.GRAVITY_AFFECT)

    if(self.isFlying) then
        self.isFlying = false
        self.parent.transparency = 255
        self.parent:toggleProperty(flagsToToggle)
        -- self.parent:say("Deinitiated flying", "TESTFONT", ChatBubbleType.SAY)
    else
        self.isFlying = true
        self.parent.transparency = 128
        self.parent:toggleProperty(flagsToToggle)
        -- self.parent:say("Initiated flying", "TESTFONT", ChatBubbleType.SAY)
    end
end

function create(parent, component)
    return Movable(parent, component)
end