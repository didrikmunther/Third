-- SpellCaster.lua

local SpellCaster = class(
	function(self, parent, component)
		self.parent = parent
		self.component = component
                          
        self.chatCtrl = nil
                          
        --[[ self.triangleTemplates = {
            {0.5, 0, 1, 0, 1},
            {1, 0, 1, 0.5, 2},
            {1, 0.5, 1, 1, 2},
            {1, 1, 0.5, 1, -1},
            {0.5, 1, 0, 1, -1},
            {0, 1, 0, 0.5, -2},
            {0, 0.5, 0, 0, -2},
            {0, 0, 0.5, 0, 1}
        } --]]

        self.SPELL_BEGIN_TIME = 1
        self.SPELL_DURATION_TIME = 2
        self.SPELL_COOLDOWN_TIME = 3
        self.SPELL_SPELL = 4
        self.SPELL_COMMANDS = 5
        self.SPELL_IS_CASTING = 6
        self.SPELL_IS_FIRST_LOOP = 7
        self.SPELL_IS_COOLDOWN = 8
        self.SPELL_ICON = 9
                          
        self.spells = {} -- {beginTime, durationTime, cooldownTime, spell, commands, isCasting, isFirstLoop, isCooldown, icon} -- icon: string (sprite name)

	end
)

function SpellCaster:addSpell(spellName, commands, durationTime, cooldownTime, icon, spellIndex)
    script = game.getScript(spellName)
    if(script == nil) then do return end end

    spell = script:getCreationFunction()(self.parent, self.component)

    self.spells[spellIndex] = {0, durationTime, cooldownTime, spell, commands, false, true, false, icon}
end

function SpellCaster:removeSpell(spellIndex)
    self.spells[spellIndex] = nil
end

function SpellCaster:clearSpells()
    for i = 1, 10 do
        self:removeSpell(i)
    end
end

function SpellCaster:cast(spellIndex)
    spell = self.spells[spellIndex]
    if(not spell or spell[self.SPELL_IS_CASTING] or spell[self.SPELL_IS_COOLDOWN]) then do return end end

    spell[self.SPELL_BEGIN_TIME] = game.getTime()
    spell[self.SPELL_IS_CASTING] = true

end

function SpellCaster:onChatCommand(commands)
    if(commands[2] == nil) then do return end end

    self:cast(tonumber(commands[2]))
end

function SpellCaster:onLoop()
    instance = self.component.instance
    if(instance.player:compare(self.parent) and not self.chatCtrl) then
        self.chatCtrl = instance.controller:getComponent("Standard/ChatController")
        if(self.chatCtrl) then
            self.chatCtrl:registerCommand("spell", self)
        end
    end

    for k,v in pairs(self.spells) do
        if(v[self.SPELL_IS_CASTING]) then
            beginTime = v[self.SPELL_BEGIN_TIME]
            durationTime = v[self.SPELL_DURATION_TIME]
            time = game.getTime()
            
            isLastLoop = false
            if(time - beginTime > durationTime) then isLastLoop = true end
            
            v[self.SPELL_SPELL]:onLoop(v[self.SPELL_IS_FIRST_LOOP], isLastLoop, v[self.SPELL_COMMANDS])
            v[self.SPELL_IS_FIRST_LOOP] = false

            if(isLastLoop) then
                v[self.SPELL_IS_CASTING] = false
                v[self.SPELL_IS_COOLDOWN] = true
                v[self.SPELL_IS_FIRST_LOOP] = true
            end
        end
        
        if(v[self.SPELL_IS_COOLDOWN]) then
            time = game.getTime()
            beginTime = v[self.SPELL_BEGIN_TIME]
            durationTime = v[self.SPELL_DURATION_TIME]
            cooldownTime = v[self.SPELL_COOLDOWN_TIME]
            v[self.SPELL_IS_COOLDOWN] = beginTime + durationTime + cooldownTime > time
        end
    end
end

function SpellCaster:onRenderAdditional()
    for k,v in pairs(self.spells) do
        if(v[self.SPELL_IS_CASTING]) then
            func = v[self.SPELL_SPELL].onRenderAdditional
            if(func) then func() end
        end
    end
    
    if(not instance.player:compare(self.parent)) then do return end end

    sW = game.getScreenWidth()
    sH = game.getScreenHeight()

    numberOfSpells = 10
    spellIconPadding = 4
    spellIconSize = 64 + spellIconPadding * 2
    spellIconMargin = 2
    spellBarW = numberOfSpells * spellIconSize + numberOfSpells * spellIconMargin + spellIconMargin

    spellBarX = (sW - spellBarW) / 2
    spellBarY = sH - spellIconSize * 2

    component = self.component
    spells = self.spells
    for i = 1,numberOfSpells do
        x = spellBarX + spellIconSize * i + spellIconMargin * i
        y = spellBarY
        
        component:renderRect(x, y, spellIconSize, spellIconSize, 150, 150, 150, 150)
        
        spell = self.spells[i]
        if(spell) then
            component:renderRect(x, y, spellIconSize, spellIconSize, 255, 255, 255, 50)
            
            spriteTransparency = 225
            if(spell[self.SPELL_IS_CASTING]) then
                component:renderRect(x, y, spellIconSize, spellIconSize, 255, 255, 255, 50)
                
                time = game.getTime();
                beginTime = spell[self.SPELL_BEGIN_TIME]
                durationTime = spell[self.SPELL_DURATION_TIME]
                percent =  (time - beginTime) / durationTime
                
                component:renderRect(x, y - (spellIconSize * percent - spellIconSize) + 1, spellIconSize, spellIconSize * percent, 0, 150, 0, 150)
            elseif(spell[self.SPELL_IS_COOLDOWN]) then
            
                time = game.getTime()
                beginTime = spell[self.SPELL_BEGIN_TIME]
                durationTime = spell[self.SPELL_DURATION_TIME]
                cooldownTime = spell[self.SPELL_COOLDOWN_TIME]
                timePassed = time - (beginTime + durationTime)
                percent = (cooldownTime - timePassed) / cooldownTime
            
                component:renderRect(x, y - (spellIconSize * percent - spellIconSize) + 1, spellIconSize, spellIconSize * percent, 150, 0, 0, 150)
                spriteTransparency = 50
            end
            
            if(spell[self.SPELL_ICON]) then
                component:renderSprite(x + spellIconPadding, y + spellIconPadding, spellIconSize - spellIconPadding * 2, spellIconSize - spellIconPadding * 2, spell[self.SPELL_ICON], spriteTransparency, false, false)
            end
        end
        
        xOffset = x + spellIconSize - spellIconPadding * 2 - 14
        yOffset = y + spellIconSize - spellIconPadding * 2 - 20
        --if(i >= 10) then xOffset = xOffset - 15 end
        
        component:renderText(xOffset, yOffset, 1, tostring(i % 10), "TESTFONT", Color(255, 255, 255, 200))
    end


end

function create(parent, component)
	return SpellCaster(parent, component)
end