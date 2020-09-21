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

class solocraft_player_instance_handler : public PlayerScript {

public:

    solocraft_player_instance_handler() : PlayerScript("solocraft_player_instance_handler") {}
    
    void OnMapChanged(Player *player) override {
        if (sWorld.GetModuleBoolConfig("Solocraft.Enable", true))
        {
            Map *map = player->GetMap();
            int difficulty = CalculateDifficulty(map, player);
            int numInGroup = GetNumInGroup(player);
            ApplyBuffs(player, map, difficulty, numInGroup);
        }
    }

private:

	std::map<uint32, int> _unitDifficulty;

    // Set the instance difficulty
    int CalculateDifficulty(Map* map, Player* /*player*/) {
        int difficulty = 1;
        if (map)
        {
            if (map->IsRaid())
                difficulty = sWorld.GetModuleIntConfig("Solocraft.Raid", 1);
            else if (map->IsHeroic())
                difficulty = sWorld.GetModuleIntConfig("Solocraft.Heroic", 1);
            else if (map->IsDungeon())
                difficulty = sWorld.GetModuleIntConfig("Solocraft.Dungeon", 1);
        }
        return difficulty;
    }

    // Get the groups size
    int GetNumInGroup(Player* player) {
        int numInGroup = 1;
        Group *group = player->GetGroup();
        if (group) {
            Group::MemberSlotList const& groupMembers = group->GetMemberSlots();
            numInGroup = groupMembers.size();
        }
        return numInGroup;
    }

    // Apply the player buffs
    void ApplyBuffs(Player* player, Map* map, int difficulty, int /*numInGroup*/)
    {
        ClearBuffs(player, map);

        if (difficulty > 1)
        {
            // InstanceMap *instanceMap = map->ToInstanceMap();
            // InstanceScript *instanceScript = instanceMap->GetInstanceScript();

            // Announce to player
            std::ostringstream ss;
            ss << "|cffFF0000[SoloCraft] |cffFF8000" << player->GetName() << " entered %s  - Difficulty Offset: %d.";
            ChatHandler(player->GetSession()).PSendSysMessage(ss.str().c_str(), map->GetMapName(), difficulty);

            // Adjust player stats
            _unitDifficulty[player->GetGUID()] = difficulty;
            for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i)
            {
                // Buff the player
                player->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, float(difficulty * 100), true);
            }

            // Set player health
            player->SetFullHealth();
            if (player->getPowerType() == POWER_MANA)
            {
                // Buff the player's health
                player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
            }
        }
    }

    void ClearBuffs(Player* player, Map* map)
    {
        std::map<uint32, int>::iterator unitDifficultyIterator = _unitDifficulty.find(player->GetGUID());
        if (unitDifficultyIterator != _unitDifficulty.end())
        {
            int difficulty = unitDifficultyIterator->second;
            _unitDifficulty.erase(unitDifficultyIterator);

            // Inform the player
            std::ostringstream ss;
            ss << "|cffFF0000[SoloCraft] |cffFF8000" << player->GetName() << " exited to %s - Reverting Difficulty Offset: %d.";
            ChatHandler(player->GetSession()).PSendSysMessage(ss.str().c_str(), map->GetMapName(), difficulty);

            // Clear the buffs
            for (int32 i = STAT_STRENGTH; i < MAX_STATS; ++i)
            {
                player->HandleStatModifier(UnitMods(UNIT_MOD_STAT_START + i), TOTAL_PCT, float(difficulty * 100), false);
            }
        }
    }
};

void AddSolocraftScripts()
{
    new SolocraftAnnounce();
    new solocraft_player_instance_handler();
}
