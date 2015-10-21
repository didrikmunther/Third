-- Npc.lua

local Npc = class (
    function(self, parent, ccomponent)
        self.parent = parent
        self.ccomponent = ccomponent

        self.target = nil

        movable = self.parent:getComponent("Standard/Movable")
        if(movable ~= nil) then
            movable.jumpPower = 2
        end
    end
)

function Npc:onLoop()
    movable = self.parent:getComponent("Standard/Movable")
    if(movable == nil) then
        do return end
    end

    target = self.target

    if(self.target ~= nil) then
        targetBox = target.body.box
        thisBox = self.parent.body.box

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

function Npc:setTarget(target)
    self.target = target
end

function create(parent, ccomponent)
    return Npc(parent, ccomponent)
end