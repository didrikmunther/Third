-- TriggerController.lua

local TriggerController = class(
    function(self, parent, component)
        self.parent = parent
        self.component = component

        self.player = nil
        self.triggers = {}
        self.renderTriggers = true
    end
)

function TriggerController:onComponentAdd(comp)
    if(comp == "Standard/TriggerController") then
        triggerController = self.parent:getComponent(comp)
        triggerController:registerTrigger(Box(0, 0, 100, 100), self)
    end
end

function TriggerController:onTrigger(inside)
    print(inside)
end


function TriggerController:registerTrigger(box, component)
    table.insert(self.triggers, {box, component, false})
end

function TriggerController:onLoop()
    if(self.player == nil) then
        self.player = self.component.instance.player
    end

    if(self.player ~= nil) then
        for k,v in pairs(self.triggers) do
            if(v[2].onTrigger) then
                if(self.player:collision(v[1])) then
                    if(not v[3]) then
                        v[3] = true
                        v[2]:onTrigger(true)
                    end
                elseif(v[3]) then
                    v[3] = false
                    v[2]:onTrigger(false)
                end
            end
        end
    end

end

function TriggerController:onRenderAdditional()
    for k,v in pairs(self.triggers) do
        self.component:renderRect(v[1].x - self.component.camera:offsetX(), v[1].y - self.component.camera:offsetY(), v[1].w, v[1].h, 255, 255, 0, 50)

        size = 2
        self.component:renderRect(v[1].x - self.component.camera:offsetX(), v[1].y - self.component.camera:offsetY(), v[1].w, size, 255, 0, 0, 100)
        self.component:renderRect(v[1].x + v[1].w - self.component.camera:offsetX(), v[1].y - self.component.camera:offsetY(), size, v[1].h, 255, 0, 0, 100)
        self.component:renderRect(v[1].x - self.component.camera:offsetX(), v[1].y + v[1].h - self.component.camera:offsetY(), v[1].w, size, 255, 0, 0, 100)
        self.component:renderRect(v[1].x - self.component.camera:offsetX(), v[1].y - self.component.camera:offsetY(), size, v[1].h, 255, 0, 0, 100)
    end
end

function create(parent, component)
    return TriggerController(parent, component)
end