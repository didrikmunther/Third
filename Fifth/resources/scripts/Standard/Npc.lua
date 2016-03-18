-- Npc.lua

local Npc = class (
    function(self, parent, component)
        self.parent = parent
        self.component = component

        self.target = nil
        self.targetName = ""
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

        targetX = 0
        targetY = targetBox.y
        if(not target:hasProperty(EntityProperty.FLIP)) then
            targetX = targetBox.x
        else
            targetX = targetBox.x + targetBox.w
        end
        targetX = targetX + (math.sin(game.getTime()) * 200) -- make them act more like bees

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

function Npc:onCollision(target, sides)
    if(target:compare(self.target)) then
        living = target:getComponent("Standard/Living")
        if(living ~= nil) then
            living:damage(1, self.parent)
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