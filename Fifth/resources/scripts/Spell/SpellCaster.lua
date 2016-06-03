-- SpellCaster.lua

local SpellCaster = class(
	function(self, parent, component)
		self.parent = parent
		self.component = component
                          
        self.chatCtrl = nil

        self.SPELL_BEGIN_TIME = 1
        self.SPELL_DURATION_TIME = 2
        self.SPELL_COOLDOWN_TIME = 3
        self.SPELL_SPELL = 4
        self.SPELL_IS_CASTING = 5
        self.SPELL_IS_COOLDOWN = 6
        self.SPELL_ICON = 7
                          
        self.spells = {} -- {beginTime, durationTime, cooldownTime, spell, isCasting, isCooldown, icon} -- icon: string (sprite name)

	end
)

function SpellCaster:onInit()
    self:addSpell("Spell/SpellHeal", 500, 3000, "frog")
end

function SpellCaster:addSpell(spellName, durationTime, cooldownTime, icon)
    script = game.getScript(spellName)
    if(script == nil) then do return end end

    spell = script:getCreationFunction()(self.parent, self.component)

    table.insert(self.spells, {0, durationTime, cooldownTime, spell, false, false, icon})
end

function SpellCaster:removeSpell(spellIndex)
    self.spells[spellIndex] = nil
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
            
            v[self.SPELL_SPELL]:onLoop()

            if(time - beginTime > durationTime) then
                v[self.SPELL_IS_CASTING] = false
                v[self.SPELL_IS_COOLDOWN] = true
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

function SpellCaster:onRender()

end

function SpellCaster:onRenderAdditional()
    sW = game.getScreenWidth()
    sH = game.getScreenHeight()

    numberOfSpells = 10
    spellIconPadding = 2
    spellIconSize = 64 + spellIconPadding * 2
    spellIconMargin = 2
    spellBarW = numberOfSpells * spellIconSize + numberOfSpells * spellIconMargin + spellIconMargin

    spellBarX = (sW - spellBarW) / 2
    spellBarY = sH - spellIconSize * 2

    component = self.component
    spells = self.spells
    for i = 0,10 do
        component:renderRect(spellBarX + spellIconSize * i + spellIconMargin * i, spellBarY, spellIconSize, spellIconSize, 150, 150, 150, 150)
        
        spell = self.spells[i+1]
        if(spell) then
            component:renderRect(spellBarX + spellIconSize * i + spellIconMargin * i, spellBarY, spellIconSize, spellIconSize, 255, 255, 255, 50)
            
            spriteTransparency = 255
            if(spell[self.SPELL_IS_CASTING]) then
                component:renderRect(spellBarX + spellIconSize * i + spellIconMargin * i, spellBarY, spellIconSize, spellIconSize, 0, 150, 0, 150)
            elseif(spell[self.SPELL_IS_COOLDOWN]) then
                component:renderRect(spellBarX + spellIconSize * i + spellIconMargin * i, spellBarY, spellIconSize, spellIconSize, 150, 0, 0, 150)
                spriteTransparency = 150
            end
            
            if(spell[self.SPELL_ICON]) then
                component:renderSprite(spellBarX + spellIconSize * i + spellIconMargin * i + spellIconPadding, spellBarY + spellIconPadding, spellIconSize - spellIconPadding * 2, spellIconSize - spellIconPadding * 2, spell[self.SPELL_ICON], spriteTransparency, false, false)
            end
        end
    end


end

function create(parent, component)
	return SpellCaster(parent, component)
end