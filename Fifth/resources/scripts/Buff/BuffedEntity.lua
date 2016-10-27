-- BuffedEntity.lua

local BuffedEntity = class(
	function(self, parent, component)
		self.parent = parent
		self.component = component

        self.BUFF_BUFF = 1
        self.BUFF_BUFF_NAME = 2
        self.BUFF_IS_FIRST_LOOP = 3
        self.BUFF_ICON = 4

        self.buffs = {} -- { name: { buff, buffName, isFirstLoop = true, icon } }
        self.buffCount = 0

        --[[
            Buff = class()
                self.isBuff = true
                self.beginTime
                self.durationTime
                self.commands
                self.onLoop

                self.onRender -- additional
            end
        --]]
	end
)

function BuffedEntity:onInit()
    self:buff("Healing", "Buff/BuffHeal", 2000, {}, "frog")
    self:buff("Healing", "Buff/BuffHeal", 3000, {}, "frog")
end

function BuffedEntity:buff(name, buffName, durationTime, commands, icon)
    overwrite = self.buffs[name] -- if it overwrites an existing buff
        
    script = game.getScript(buffName)
    if(script == nil) then do return end end

    buffFunc = script:getCreationFunction()
    if(not buffFunc) then do return end end
    buff = buffFunc(self.parent, self.component)
    if(not buff.isBuff and not buff.isDebuff) then do return end end

    beginTime = game.getTime()
    buff.beginTime = beginTime
    buff.durationTime = durationTime
    buff.commands = commands

    if(buff.onInit) then buff:onInit() end

    self.buffs[name] = {buff, buffName, true, icon}
    self.buffCount = self.buffCount + 1

    if(overwrite) then self.buffCount = self.buffCount - 1 end
end

function BuffedEntity:onLoop()

    for k,v in pairs(self.buffs) do

        time = game.getTime()
        isLastLoop = (time - v[self.BUFF_BUFF].beginTime > v[self.BUFF_BUFF].durationTime)

        v[self.BUFF_BUFF]:onLoop(v[self.BUFF_IS_FIRST_LOOP], isLastLoop)
        v[self.BUFF_IS_FIRST_LOOP] = false

        if(isLastLoop) then
            self.buffs[k] = nil
            self.buffCount = self.buffCount - 1
        end
    end
end

function BuffedEntity:onRenderAdditional()

    sW = game.getScreenWidth()
    sH = game.getScreenHeight()

    buffIconPadding = 4
    buffIconSize = 64 + buffIconPadding * 2
    buffIconMargin = 2
    buffSideMargin = buffIconSize / 4

    buffIconBarW = self.buffCount * buffIconSize + self.buffCount * buffIconMargin + buffIconMargin
    buffIconBarX = sW - buffIconBarW - buffSideMargin
    buffIconBarY = 0 + buffSideMargin

    component = self.component
    i = 0
    for k,v in pairs(self.buffs) do
        
        x = buffIconBarX + buffIconSize * i + buffIconMargin * i
        y = buffIconBarY
        
        component:renderRect(x, y, buffIconSize, buffIconSize, 150, 150, 150, 150)
        
        time = game.getTime()
        durationTime = v[self.BUFF_BUFF].durationTime
        beginTime = v[self.BUFF_BUFF].beginTime
        percent =  (time - beginTime) / durationTime
        component:renderRect(x, y - (buffIconSize * percent - buffIconSize) + 1, buffIconSize, buffIconSize * percent, 0, 150, 0, 150)
        
        if(v[self.BUFF_ICON]) then
            spriteTransparency = 200
            component:renderSprite(x + buffIconPadding, y + buffIconPadding, buffIconSize - buffIconPadding * 2, buffIconSize - buffIconPadding * 2, v[self.BUFF_ICON], spriteTransparency, false, false)
        end
        
        func = v[self.BUFF_BUFF].onRenderAdditional
        if(func) then func() end
        
        i = i + 1
    end

end

function create(parent, component)
	return BuffedEntity(parent, component)
end