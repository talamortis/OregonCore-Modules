#include <map>
#include "Config.h"
#include "ScriptMgr.h"
#include "Unit.h"
#include "Player.h"
#include "Pet.h"
#include "Map.h"
#include "Group.h"
#include "InstanceData.h"
#include "Chat.h"
#include "Instances/InstanceData.h"
#include "Object.h"
#include "ObjectAccessor.h"
#include <vector>

// Set the instance difficulty
int CalculateDifficulty(Map* map, Player* player) {
    int difficulty = 1;
    if (map) {
        if (map->IsHeroic()) {
            difficulty = sWorld.GetModuleIntConfig("Solocraft.Heroic", 5);
        }
        else if (map->IsRaid()) {
            difficulty = sWorld.GetModuleIntConfig("Solocraft.Raid", 5);
        }
        else if (map->IsDungeon()) {
            difficulty = sWorld.GetModuleIntConfig("Solocraft.Dungeon", 5);
        }
    }
    return difficulty;
}

class SolocraftAnnounce : public PlayerScript
{
public:

    SolocraftAnnounce() : PlayerScript("SolocraftAnnounce") {}

    void OnLogin(Player* player, bool OnFirstLogin)
    {
        // Announce Module
        if (sWorld.GetModuleBoolConfig("Solocraft.Enable", 1))
        {
            if (sWorld.GetModuleBoolConfig("Solocraft.Announce", 1))
            {
                ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00SoloCraft |rmodule.");
            }
        }
    }
};

class solocraft_uint_script : public UnitScript
{
public:
    solocraft_uint_script() : UnitScript("solocraft_uint_script") {}

    void OnHeal(Unit* unit, Unit* reciever, uint32& gain)
    {
        if (sWorld.GetModuleBoolConfig("Solocraft.Enable", true))
        {
            Player* plr = unit->ToPlayer();
            Pet* Unitpet = unit->ToPet();

            if (Unitpet)
                if (Unitpet->GetOwner()->GetTypeId() == TYPEID_PLAYER)
                    gain *= CalculateDifficulty(Unitpet->GetMap(), Unitpet->GetOwner());

            if (plr)
                if (plr)
                    gain *= CalculateDifficulty(plr->GetMap(), plr);

        }
    }

    void OnDealDamage(Unit* unit, uint32& damage)
    {
        if (sWorld.GetModuleBoolConfig("Solocraft.Enable", true))
        {
            Player* plr = unit->ToPlayer();
            Pet* Unitpet = unit->ToPet();

            if (Unitpet)
                if (Unitpet->GetOwner()->GetTypeId() == TYPEID_PLAYER)
                    damage *= CalculateDifficulty(Unitpet->GetMap(), Unitpet->GetOwner());

            if (plr)
                if (plr)
                    damage *= CalculateDifficulty(plr->GetMap(), plr);
        }
    }
};

class solocraft_player_instance_handler : public PlayerScript {

public:

    solocraft_player_instance_handler() : PlayerScript("solocraft_player_instance_handler") {}

    std::string name = "";

	void OnMapChanged(Player *player) override {
		if (sWorld.GetModuleBoolConfig("Solocraft.Enable", true)) {
			Map* map = player->GetMap();
			if (!player->IsAlive())
			{
				player->ResurrectPlayer(0.7f);
				player->GetCorpse();
				player->SpawnCorpseBones();
			} 
			int difficulty = CalculateDifficulty(map, player);
			int numInGroup = GetNumInGroup(player);
			ApplyBuffs(player, map, difficulty, numInGroup);
		}
	}

private:

    std::map<uint32, int> _unitDifficulty;
    int diff_power_multiplier = sWorld.GetModuleIntConfig("Solocraft.Diff_Power_Multiplier", 100);

    int GetNumInGroup(Player* player) {
        int numInGroup = 1;
        Group* group = player->GetGroup();
        if (group) {
            Group::MemberSlotList const& groupMembers = group->GetMemberSlots();
            numInGroup = groupMembers.size();
        }
        return numInGroup;
    }
	
    std::vector<uint32> spells = { 25392 , 32999 ,25433, 25898, 27127, 26991 };

    // Apply the player buffs
    void ApplyBuffs(Player* player, Map* map, int difficulty, int numInGroup) {
        ClearBuffs(player, map);
        if (difficulty > 1) {
            //InstanceMap *instanceMap = map->ToInstanceMap();
            //InstanceScript *instanceScript = instanceMap->GetInstanceScript();

            std::ostringstream ss;
            ss << "|cffFF0000[SoloCraft] |cffFF8000" << player->GetName() << " entered %s  - Difficulty Offset: %d.";
            ChatHandler(player->GetSession()).PSendSysMessage(ss.str().c_str(), map->GetMapName(), difficulty);

            ChatHandler(player->GetSession()).PSendSysMessage("Welcome to %s ! You got superpowers!",
                map->GetMapName(), difficulty, numInGroup);

            _unitDifficulty[player->GetGUID()] = difficulty;
            for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i) {
				player->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_VALUE, float((player->getLevel()) * difficulty * diff_power_multiplier), true);
            }

            player->SetFullHealth();

            if (player->getPowerType() == POWER_MANA) {
                player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
            }

            if (sWorld.GetModuleBoolConfig("Solocraft.CooldownCheat", true)) {
                player->SetCommandStatusOn(CHEAT_COOLDOWN);
                ChatHandler(player->GetSession()).PSendSysMessage("Cooldown Cheat is ON. You are not on the global cooldown.");
            }

            if (sWorld.GetModuleBoolConfig("Solocraft.CastTimeCheat", true)) {
                player->SetCommandStatusOn(CHEAT_CASTTIME);
                ChatHandler(player->GetSession()).PSendSysMessage("CastTime Cheat is ON. Your spells won't have a casttime.");
            }

            if (sWorld.GetModuleBoolConfig("Solocraft.Buffs", true)) {
                for (int i = 0; i < spells.size(); ++i)
                    player->CastSpell(player, spells[i], true);
                ChatHandler(player->GetSession()).PSendSysMessage("|cffB400B4You have been buffed, enjoy!");
            }


        }
    }

    void ClearBuffs(Player* player, Map* map) {
        std::map<uint32, int>::iterator unitDifficultyIterator = _unitDifficulty.find(player->GetGUID());
        if (unitDifficultyIterator != _unitDifficulty.end()) {
            int difficulty = unitDifficultyIterator->second;
            _unitDifficulty.erase(unitDifficultyIterator);

			// Inform the player
			std::ostringstream ss;
			ss << "|cffFF0000[SoloCraft] |cffFF8000" << player->GetName() << " exited to %s - Reverting Difficulty Offset: %d.";
			ChatHandler(player->GetSession()).PSendSysMessage(ss.str().c_str(), map->GetMapName(), difficulty);

            for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i) {
                player->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_VALUE, float((player->getLevel()) * difficulty * diff_power_multiplier), false);
            }

            if (sWorld.GetModuleBoolConfig("Solocraft.CooldownCheat", true)) {
                player->SetCommandStatusOff(CHEAT_COOLDOWN);
                ChatHandler(player->GetSession()).PSendSysMessage("Cooldown Cheat is OFF. You are on the global cooldown.");
            }

            if (sWorld.GetModuleBoolConfig("Solocraft.CastTimeCheat", true)) {
                player->SetCommandStatusOff(CHEAT_CASTTIME);
                ChatHandler(player->GetSession()).PSendSysMessage("CastTime Cheat is OFF. Your spells will have a casttime.");
            }

            if (sWorld.GetModuleBoolConfig("Solocraft.Buffs", true))
            {
                for (int i = 0; i < spells.size(); i++)
                    player->RemoveAurasDueToSpell(spells[i]);
                ChatHandler(player->GetSession()).PSendSysMessage("|cffB400B4Your dungeon buffs removed!");
            }
        }
    }
};

void AddSolocraftScripts()
{
    new solocraft_uint_script();
    new SolocraftAnnounce();
    new solocraft_player_instance_handler();
}
