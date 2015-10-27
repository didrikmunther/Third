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

function Movable:setMovementState(movementState)
    self.movementState = movementState
end

function Movable:toggleNoClip()

    --movable = self.parent:getComponent("Standard/Movable")  -- Test getting components
    --if(movable == self) then
    --    print("hello")
    --end

    --tempEntity = self.parent.entityManager:createColoredEntity(Box(100, 100, 200, 200), Color(255, 0, 255, 255))
    --script = game.getScript("Standard/Movable")
    --tempEntity:addComponent(script)
    --self.parent.entityManager:addEntity(tempEntity, "")

    flagsToToggle = BitOR(EntityProperty.COLLIDABLE, EntityProperty.GRAVITY_AFFECT)

    if(self.isFlying) then
        self.isFlying = false
        self.parent.transparency = 255
        self.parent.properties = BitOR(flagsToToggle, self.parent.properties) -- toggle flag on
        self.parent:say("Deinitiated flying", "TESTFONT", ChatBubbleType.SAY)
    else
        self.isFlying = true
        self.parent.transparency = 128
        self.parent.properties = BitAND(BitNOT(flagsToToggle), self.parent.properties) -- toggle flag off
        self.parent:say("Initiated flying", "TESTFONT", ChatBubbleType.SAY)
    end
end

function create(parent, component)
    return Movable(parent, component)
end