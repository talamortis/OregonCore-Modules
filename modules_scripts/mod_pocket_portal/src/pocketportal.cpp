/* Credits: Micrah/Milestorme Module creator */

#include "ScriptPCH.h"
#include "World.h"
#include "GossipDef.h"
#include "Item.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "Spell.h"
#include "Config.h"
#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "Map.h"

enum Vendors
{
    NPC_VENDOR_A    = 100000,
    NPC_VENDOR_H    = 100000,
};



class pocket_portal : public ItemScript
{
public:
    pocket_portal() : ItemScript("pocket_portal") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        if (!sWorld.GetModuleBoolConfig("PocketPortal.Enable", true))
            return false;  

       if (player->duel || player->GetMap()->IsBattleArena() || player->InBattleground() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || player->isDead()|| player->IsInCombat() || player->IsInFlight() || player->HasStealthAura() || player->HasInvisibilityAura())
           return false;

        float rangeCheck = 10.0f;
        if (player->FindNearestCreature(NPC_VENDOR_A, rangeCheck) ||
            player->FindNearestCreature(NPC_VENDOR_H, rangeCheck))
            return false;
	    
        player->PlayerTalkClass->ClearMenus();

        if (sWorld.GetModuleBoolConfig("PocketPortal.Enable", true))
           player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface/ICONS/INV_Misc_Map08:35:35:-23:0|t-> Pocket Portal", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
        return false; // Cast the spell on use normally
    }
void OnGossipSelect(Player* player, Item* /*item*/, uint32 /*sender*/, uint32 action) override
    {
        switch (action)
        {
             case GOSSIP_ACTION_INFO_DEF + 1: /*portal*/
            {
                uint32 vendorId = 0;
                std::string salute;
                if (player->GetTeamId() == TEAM_ALLIANCE)
                {
                    vendorId = NPC_VENDOR_A;
                    salute = "Greetings";
                } else {
                    vendorId = NPC_VENDOR_H;
                    salute = "Zug zug";
                }

                SummonTempNPC(player, vendorId, salute.c_str());
                player->CLOSE_GOSSIP_MENU();
                break;
            }
			
		}
    }
			
 
    void SummonTempNPC(Player* player, uint32 entry, const char* salute = "")
    {
        if (!player || entry == 0)
            return;

        int npcDuration = sWorld.GetModuleIntConfig("Portal.NpcDuration", 60) * IN_MILLISECONDS;
        if (npcDuration <= 0) // Safeguard
            npcDuration = 60;

        Creature* npc = player->SummonCreature(entry, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, npcDuration);
        npc->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        npc->GetMotionMaster()->MoveFollow(player, PET_FOLLOW_DIST, player->GetFollowAngle());
        npc->SetFaction(player->GetFaction());

		//if (salute && !(salute[0] == '\0'))
        //    npc->MonsterWhisper(salute, player, false);
    }
};




void AddSC_pocket_portal()
{
    new pocket_portal();
}
