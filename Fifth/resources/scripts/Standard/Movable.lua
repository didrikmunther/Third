-- Movable.lua

Movable = class (
    function(self, parent)
        self.parent = parent

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
    body = self.parent.body

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

    movable = self.parent:getComponent("Standard/Movable")  -- Test getting components
    if(movable == self) then
        print("hello")
    end

    if(self.isFlying) then
        self.isFlying = false
        self.parent.properties = BitOR(EntityProperty.COLLIDABLE, self.parent.properties) -- toggle flag on
    else
        self.isFlying = true
        self.parent.properties = BitAND(BitNOT(EntityProperty.COLLIDABLE), self.parent.properties) -- toggle flag off
    end
end

function create(parent)
    return Movable(parent)
end