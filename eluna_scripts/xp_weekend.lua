--[[ Double XP weekend script for ELUNA

Written by Nix
Discord: Nix#4045
Website: http://novuscore.org
Github: https://github.com/NixAJ

Edited By Talamortis
Discord: Speedfangz#6864
Github: https://github.com/talamortis
]]


local function OnGivenXP(event, player, amount, victim)
	local day = os.date("*t").wday
    if day == 6 or day == 7 or day == 1 then
        amount = amount * 2
    end
	
	return amount
end

local function OnLogin(event, player)

	local day = os.date("*t").wday
    if day == 6 or day == 7 or day == 1 then
		player:SendBroadcastMessage("|cffff0000[Announcement]|r It's the Weekend! Double XP is |cFFADFF2FEnabled|r! ")
	end
		
end

RegisterPlayerEvent(12, OnGivenXP)
RegisterPlayerEvent(3, OnLogin)