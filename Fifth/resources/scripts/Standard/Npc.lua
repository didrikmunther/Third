-- Npc.lua

local Npc = class (
    function(self, parent, component)
        self.parent = parent
        self.component = component

        self.target = nil
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

function Npc:onRender()
    camera = self.component.camera
    cameraX = camera:offsetX()
    cameraY = camera:offsetY()

    box = self.parent.body.box
    thisX = box.x - cameraX
    thisY = box.y - cameraY
    thisW = box.w
    thisH = box.h

    component = self.component
    lineWidth = 2

    component:renderRect(thisX, thisY, thisW, lineWidth, 255, 100, 255, 255)
    component:renderRect(thisX, thisY, lineWidth, thisH, 255, 100, 255, 255)
    component:renderRect(thisX + thisW, thisY, lineWidth, thisH, 255, 100, 255, 255)
    component:renderRect(thisX, thisY + thisH, thisW, lineWidth, 255, 100, 255, 255)


    --tBox = self.target.body.box
    --tX = tBox.x
    --tY = tBox.y

    --component:renderLine(thisX, thisY, tX, tY, 255, 100, 100, 255)
end

function Npc:onDeserialize(value)
    decoded = json.decode(value)

    if(decoded.target ~= nil) then
        self.target = getVal(self.target, self.parent.entityManager:getEntity(decoded.target))
    end
end

function create(parent, component)
    return Npc(parent, component)
end