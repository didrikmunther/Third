-- Shooter.lua

local Shooter = class(
    function(self, parent, component)
        self.parent = parent
        self.component = component
    end
)

function Shooter:shoot()
    -- game:leftMousePressed() and 

    if(not self.parent.isDead) then
        box = self.parent.body.box
        thisX = box.x
        thisY = box.y
        thisW = box.w

        for i = 0, 0 do
            spawnX = thisX + thisW - 10
            spawnY = thisY + 68
            if(self.parent:hasProperty(EntityProperty.FLIP)) then
                spawnX = spawnX - thisW
            end

            --bullet = self.parent.entityManager:createColoredEntity(Box(spawnX, spawnY, 14, 10), Color(255, 5, 0, 255))
            bullet = self.parent.entityManager:createSpriteEntity(Box(spawnX, spawnY, 12, 6), "bullet")
            self.parent.entityManager:addParticle(bullet)
            script = game.getScript("Standard/Projectile")
            bullet:addComponent(self.component.instance, script)
            bullet:getComponent("Standard/Projectile"):onDeserialize('{"owner":"' .. self.parent.entityManager:getNameOfEntity(self.parent) .. '"}')

            tBody = self.parent.body
            bBody = bullet:getComponent("Standard/Projectile").parent.body -- must do it this way

            bBody.velX = 25 + (math.random(5) - 2.5)
            bBody.velY = -5 + (math.random(5) - 2.5)
            if(self.parent:hasProperty(EntityProperty.FLIP)) then
                bBody.velX = bBody.velX * -1
            end

            bBody.velX = bBody.velX + tBody.velX
            bBody.velY = bBody.velY + tBody.velY

        end
    end
end

function create(parent, component)
    return Shooter(parent, component)
end