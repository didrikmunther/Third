-- Movable.lua

local Movable = class (
    function(self, parent, component)
        self.parent = parent
        self.component = component
        self.body = parent.body

        self.jumpPower = 16.7
        self.accelerationX = 1.5
        self.accelerationY = 100.0
        self.stoppingAccelerationX = 1.5
        self.stoppingAccelerationY = 100.0
                       
        self.hasWalkedX = false
        self.hasWalkedY = false

        self.isFlying = false
        self.angleFactor = -1.0

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

    if(body.velX ~= 0 and self.parent.collisionSides.bottom) then
        self.parent:setSprite(self.parent:getSpriteFromState("WALKING"))
    end

    if(self.isFlying) then
        self.parent:setSprite(self.parent:getSpriteFromState("FLYING"))
    end

    self.parent.angle = (body.velX / -self.angleFactor)

end

function Movable:onEvent(key, keyDown)
    if(keyDown) then
        if(key == KeyCode._1) then
            self:toggleNoClip()
        end

        if(key == KeyCode._2) then
            self.parent:toggleProperty(EntityProperty.HIDDEN)
        end

        if(key == toKeyCode(ScanCode._LSHIFT)) then
            self.movementState = self.SNEAKING_MOVEMENT
            self.parent.fpsFactor = 0.8
        end

        if(key == toKeyCode(ScanCode._LALT)) then
            self.movementState = self.RUNNING_MOVEMENT
            self.parent.fpsFactor = 2
        end

    else
        if(key == toKeyCode(ScanCode._LALT) or key == toKeyCode(ScanCode._LSHIFT)) then
            self.movementState = self.WALKING_MOVEMENT
            self.parent.fpsFactor = 1
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
end

function Movable:onSerialize()
    c = self.component

    c:addFloat("walking_movement_speed", self.movementSpeeds[self.WALKING_MOVEMENT])
    c:addFloat("running_movement_speed", self.movementSpeeds[self.RUNNING_MOVEMENT])
    c:addFloat("sneaking_movement_speed", self.movementSpeeds[self.SNEAKING_MOVEMENT])

    isFlying = 0
    if(self.isFlying) then isFlying = 1 end
    c:addInt("isFlying", isFlying)
end

function Movable:onDeserialize(value)
    decoded = json.decode(value)

    self.movementSpeeds[self.WALKING_MOVEMENT] = getVal(self.movementSpeeds[self.WALKING_MOVEMENT], decoded.walking_movement_speed)
    self.movementSpeeds[self.RUNNING_MOVEMENT] = getVal(self.movementSpeeds[self.RUNNING_MOVEMENT], decoded.running_movement_speed)
    self.movementSpeeds[self.SNEAKING_MOVEMENT] = getVal(self.movementSpeeds[self.SNEAKING_MOVEMENT], decoded.sneaking_movement_speed)
    self.jumpPower = getVal(self.jumpPower, decoded.jumpPower)
    self.movementState = getVal(self.movementState, decoded.movementState)

    if(decoded.isFlying ~= nil) then
        if(decoded.isFlying == 1) then
            if(not self.isFlying) then
                self.isFlying = false
                self:toggleNoClip()
            end
        else
            if(self.isFlying) then
                self.isFlying = true
                self:toggleNoClip()
            end
        end
    end
    
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

function Movable:doJump()
    body = self.parent.body

    body.velY = body.velY - self.accelerationY
    if(body.velY < -self.jumpPower) then
        body.velY = -self.jumpPower
    end
end

function Movable:jump()
	if(self.parent.collisionSides.bottom) then
        self:doJump()
    end

end

function Movable:toggleNoClip()

    flagsToToggle = BitOR(EntityProperty.COLLIDABLE, EntityProperty.GRAVITY_AFFECT)

    if(self.isFlying) then
        self.isFlying = false
        self.parent.transparency = 255
        self.parent:addProperty(flagsToToggle)
        -- self.parent:say("Deinitiated flying", "TESTFONT", ChatBubbleType.SAY)
    else
        self.isFlying = true
        self.parent.transparency = 128
        self.parent:removeProperty(flagsToToggle)
        -- self.parent:say("Initiated flying", "TESTFONT", ChatBubbleType.SAY)
    end
end

function create(parent, component)
    return Movable(parent, component)
end
