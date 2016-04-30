-- Npc.lua

local Npc = class (
    function(self, parent, component)
        self.parent = parent
        self.component = component

        self.target = nil
        self.targetName = ""
        self.range = 50
        self.chaseSpeed = 10

        self.roam = true
        self.roamSpeed = 1
        self.lastTime = game.getTime()
        self.standing = true
        self.standTimer = 0
        self.standTime = 1100
        self.walkingTimer = 0
        self.walkingTime = 1000
        self.walkingDirection = true -- right
    end
)

function Npc:onInit()
    movable = self.parent:getComponent("Standard/Movable")
    if(movable ~= nil) then
        movable.jumpPower = 2
    end
end

function Npc:onLoop()
    movable = self.parent:getComponent("Standard/Movable")
    if(movable == nil) then
        do return end
    end

    target = self.target

    if(self.target == nil and self.targetName ~= "") then
        self.target = self.parent.entityManager:getEntity(self.targetName)
    end

    if(target ~= nil) then
        targetBox = target.body.box
        thisBox = self.parent.body.box

        if(math.sqrt(math.pow(thisBox.x - targetBox.x, 2) + math.pow(thisBox.y - targetBox.y, 2)) > self.range) then

            if(self.roam) then
                movable.movementSpeeds[movable.WALKING_MOVEMENT] = self.roamSpeed

                time = game.getTime()
                dTime = time - self.lastTime
                self.lastTime = time

                if(self.standing) then
                    self.standTimer = self.standTimer + dTime
                    if(self.standTimer > self.standTime) then
                        self.standing = false
                        self.standTimer = 0
                    end
                else
                    self.walkingTimer = self.walkingTimer + dTime
                    if(self.walkingTimer > self.walkingTime) then
                        self.walkingTimer = 0
                        self.standing = true
                    end

                    if(math.random(50) == 1) then
                        self.walkingDirection = math.random(100) < 50
                    end

                    if(self.walkingDirection) then
                        movable:goRight()
                    else
                        movable:goLeft()
                    end
                end
            end
        else
            movable.movementSpeeds[movable.WALKING_MOVEMENT] = self.chaseSpeed
            targetX = 0
            targetY = targetBox.y
            if(not target:hasProperty(EntityProperty.FLIP)) then
                targetX = targetBox.x
            else
                targetX = targetBox.x + targetBox.w
            end
            targetX = targetX + (math.sin(game.getTime()) * 100) -- make them act more like bees

            thisX = thisBox.x
            thisY = thisBox.y

            if(targetX > thisX) then
                movable:goRight()
            elseif(targetX < thisX) then
                movable:goLeft()
            end

            if(thisY > targetY) then
                targetMovable = self.target:getComponent("Standard/Movable")
                if(targetMovable ~= nil) then
                    if(not targetMovable.isFlying) then
                        movable:jump()
                    end
                else
                    movable:jump()
                end
            end
        end
    end
end

function Npc:onCollision(target, sides)
    if(target:compare(self.target)) then
        living = target:getComponent("Standard/Living")
        if(living ~= nil) then
            living:damage(50, self.parent)
        end
    end
end

function Npc:onSerialize()
    c = self.component

    c:addString("target", self.parent.entityManager:getNameOfEntity(self.target))
end

function Npc:onDeserialize(value)
    decoded = json.decode(value)

    if(decoded.target ~= nil) then
        self.target = getVal(self.target, self.parent.entityManager:getEntity(decoded.target))
        self.targetName = decoded.target
    end

end

function create(parent, component)
    return Npc(parent, component)
end