#include <map>

#include "Config.h"
#include "ScriptMgr.h"
#include "Unit.h"
#include "Player.h"
#include "Pet.h"
#include "Map.h"
#include "Group.h"
#include "InstanceSaveMgr.h"
#include "Log.h"
#include "Chat.h"
#include "WorldSession.h"
#include "SpellMgr.h"
#include "Spell.h"


namespace {

class solocraft_player_instance_handler : public PlayerScript {
public:
	solocraft_player_instance_handler() : PlayerScript("solocraft_player_instance_handler") {
		sLog.outString("scripts.solocraft.player.instance", "[Solocraft] solocraft_player_instance_handler Loaded");
	}
	

	
	void OnLogin(Player *player, bool firstLogin) override {
		if (sWorld.GetModuleBoolConfig("Solocraft.Announce", true))
		{
		ChatHandler(player->GetSession()).SendSysMessage("Solocraft mode activated in dungeons and raids");
		}
	}
	


	void OnMapChanged(Player *player) override {
		if (sWorld.GetModuleBoolConfig("Solocraft.Enable", true)) {
		Map* map = player->GetMap();
            if (!player->IsAlive())
            {
                player->ResurrectPlayer(0.7f);
            }
		int difficulty = CalculateDifficulty(map, player);
		int numInGroup = GetNumInGroup(player);
		ApplyBuffs(player, map, difficulty, numInGroup);
		}
	}
	
	
private:
	std::map<ObjectGuid, int> _unitDifficulty;

	int diff_power_multiplier = sWorld.GetModuleIntConfig("Solocraft.Diff_Power_Multiplier", 100);

	
	int CalculateDifficulty(Map *map, Player *player) {
		int difficulty = 1;
		if (map) {
			 if (map->IsHeroic()) {
				difficulty = 10;
			} else if (map->IsRaid()) {
				difficulty = 25;
			} else if (map->IsDungeon()) {
				difficulty = 5;
			}
		}
		return difficulty;
	}

	int GetNumInGroup(Player *player) {
		int numInGroup = 1;
		Group *group = player->GetGroup();
		if (group) {
			Group::MemberSlotList const& groupMembers = group->GetMemberSlots();
			numInGroup = groupMembers.size();
		}
		return numInGroup;
	}

	
	enum spells
	{
		POWER_WORD_FORTITUDE = 25392,
		PRAYER_OF_SPRITE = 32999,
		SHADOW_BUFF = 25433,
		KINGS_BUFF = 25898,
		ARCANE_BUFF = 27127,
		MARK_OF_THE_WILD = 26991,
	};	
	
	
	void ApplyBuffs(Player *player, Map *map, int difficulty, int numInGroup) {
		ClearBuffs(player, map);
		if (difficulty > 1) {
			//InstanceMap *instanceMap = map->ToInstanceMap();
			//InstanceScript *instanceScript = instanceMap->GetInstanceScript();

			ChatHandler(player->GetSession()).PSendSysMessage("Welcome to %s ! You got superpowers!",
					map->GetMapName(), difficulty, numInGroup);

			_unitDifficulty[player->GetGUID()] = difficulty;
			for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i) {
				player->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_VALUE, float((player->getLevel())*difficulty * diff_power_multiplier), true);
			}
			
			//player->ApplySpellPowerBonus((player->getLevel())*difficulty*diff_power_multiplier, true);
			
			player->SetFullHealth();
			
			if (player->getPowerType() == POWER_MANA) {
				player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
			}
			
			if (sWorld.GetModuleBoolConfig("Solocraft.CooldownCheat", true)){
				player->SetCommandStatusOn(CHEAT_COOLDOWN);
				ChatHandler(player->GetSession()).PSendSysMessage("Cooldown Cheat is ON. You are not on the global cooldown.");			
			}			
			
			if (sWorld.GetModuleBoolConfig("Solocraft.CastTimeCheat", true)){
				player->SetCommandStatusOn(CHEAT_CASTTIME);
				ChatHandler(player->GetSession()).PSendSysMessage("CastTime Cheat is ON. Your spells won't have a casttime.");			
			}	
			
			if (sWorld.GetModuleBoolConfig("Solocraft.Buffs", true)){
				for(int i = 0; i < 7; i++)
				player->CastSpell(player, POWER_WORD_FORTITUDE, true);
				player->CastSpell(player, KINGS_BUFF, true);
				player->CastSpell(player, MARK_OF_THE_WILD, true);
				player->CastSpell(player, PRAYER_OF_SPRITE, true);
				player->CastSpell(player, ARCANE_BUFF, true);
				player->CastSpell(player, SHADOW_BUFF, true);
				ChatHandler(player->GetSession()).PSendSysMessage("|cffB400B4You have been buffed, enjoy!");		
			}
			
			
		}
	}

	void ClearBuffs(Player *player, Map *map) {
		std::map<ObjectGuid, int>::iterator unitDifficultyIterator = _unitDifficulty.find(player->GetGUID());
		if (unitDifficultyIterator != _unitDifficulty.end()) {
			int difficulty = unitDifficultyIterator->second;
			_unitDifficulty.erase(unitDifficultyIterator);

			ChatHandler(player->GetSession()).PSendSysMessage("Superpowers turned off.)",
					map->GetMapName(), difficulty);

			for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i) {
				player->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_VALUE, float((player->getLevel())* difficulty * diff_power_multiplier), false);
			}
			
			if (sWorld.GetModuleBoolConfig("Solocraft.CooldownCheat", true)){			
				player->SetCommandStatusOff(CHEAT_COOLDOWN);
				ChatHandler(player->GetSession()).PSendSysMessage("Cooldown Cheat is OFF. You are on the global cooldown.");
			}	
							
			if (sWorld.GetModuleBoolConfig("Solocraft.CastTimeCheat", true)){			
				player->SetCommandStatusOff(CHEAT_CASTTIME);
				ChatHandler(player->GetSession()).PSendSysMessage("CastTime Cheat is OFF. Your spells will have a casttime.");
			}	
			
			if (sWorld.GetModuleBoolConfig("Solocraft.Buffs", true)){
				for(int i = 0; i < 7; i++)
				player->RemoveAurasDueToSpell(POWER_WORD_FORTITUDE);
				player->RemoveAurasDueToSpell(KINGS_BUFF);
				player->RemoveAurasDueToSpell(MARK_OF_THE_WILD);
				player->RemoveAurasDueToSpell(PRAYER_OF_SPRITE);
				player->RemoveAurasDueToSpell(ARCANE_BUFF);
				player->RemoveAurasDueToSpell(SHADOW_BUFF);
				ChatHandler(player->GetSession()).PSendSysMessage("|cffB400B4Your dungeon buffs removed!");		
			}
			
			//player->ApplySpellPowerBonus((player->getLevel())*difficulty*diff_power_multiplier, false);
			
				
		}
	}
};

}


void AddSolocraftScripts() 
{
	new solocraft_player_instance_handler();
}