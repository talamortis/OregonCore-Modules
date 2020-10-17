/* ScriptData
SDName: NPC EXCHANGER
SD%Complete: 100
SDComment: By Evrial
SDCategory: NPC
EndScriptData */

#include "ScriptPCH.h"
#include "Config.h"
#include "World.h"
#include "Chat.h"
#include "SystemConfig.h"
#include <cstring>

class npc_exchanger : public CreatureScript
{
public:
    npc_exchanger() : CreatureScript("npc_exchanger") {}


bool OnGossipHello(Player* pPlayer, Creature* pCreature)
{
    if (sWorld.GetModuleBoolConfig("NPC_Exchanger.Enable", true))
    {
	for (uint32 i = 1; i <= 12; i++)
	{
		QueryResult* type = WorldDatabase.PQuery("SELECT `text1`, `action` FROM `npc_exchanger` WHERE `type` = %u AND `active` = 1", i);
		if (type)
		{
			Field *fields = type->Fetch();
			pPlayer->ADD_GOSSIP_ITEM( 9, fields[0].GetString(), GOSSIP_SENDER_MAIN, fields[1].GetInt32());
		}
	}

	pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
	return true;
    }
}

void SendDefaultMenu(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{
    // Not allowed if in combat.
    if (pPlayer->IsInCombat())
    {
      pPlayer->CLOSE_GOSSIP_MENU();
      pCreature->MonsterSay("You are in combat!", LANG_UNIVERSAL, NULL);
      return;
    }

	switch(uiAction)
    {
	case 1001:
		{
		QueryResult* type1 = WorldDatabase.PQuery("SELECT `data1`, `data2`, `data3`, `text2` FROM `npc_exchanger` WHERE `type` = 1");
		Field *fields1 = type1->Fetch();
		if (pPlayer->GetHonorPoints() >= fields1[0].GetInt32())
		{
			pPlayer->ModifyHonorPoints((-1)*fields1[0].GetInt32());
			pPlayer->ModifyArenaPoints(fields1[1].GetInt32());
			pCreature->MonsterWhisper(fields1[3].GetString(), pPlayer->GetGUID());
		}
		else
			pCreature->MonsterWhisper("You have not enough Honor Points", pPlayer->GetGUID());
		}
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1002:
		{
		QueryResult* type2 = WorldDatabase.PQuery("SELECT `data1`, `data2`, `data3`, `text2` FROM `npc_exchanger` WHERE `type` = 2");
		Field *fields2 = type2->Fetch();
		if (pPlayer->GetArenaPoints() >= fields2[0].GetInt32())
		{
			pPlayer->ModifyArenaPoints((-1)*fields2[0].GetInt32());
			pPlayer->ModifyHonorPoints(fields2[1].GetInt32());
			pCreature->MonsterWhisper(fields2[3].GetString(), pPlayer->GetGUID());
		}
		else
			pCreature->MonsterWhisper("You have not enough Arena Points", pPlayer->GetGUID());
		}
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1003:
		{
		QueryResult* type3 = WorldDatabase.PQuery("SELECT `data1`, `data2`, `data3`, `text2` FROM `npc_exchanger` WHERE `type` = 3");
		Field *fields3 = type3->Fetch();
		if (pPlayer->GetHonorPoints() >= fields3[0].GetInt32())
		{
			pPlayer->ModifyHonorPoints((-1)*fields3[0].GetInt32());
			pPlayer->StoreNewItemInBestSlots(fields3[2].GetInt32(), fields3[1].GetInt32());
			pCreature->MonsterWhisper(fields3[3].GetString(), pPlayer->GetGUID());
		}
		else
			pCreature->MonsterWhisper("You have not enough Honor Points", pPlayer->GetGUID());
		}
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1004:
		{
		QueryResult* type4 = WorldDatabase.PQuery("SELECT `data1`, `data2`, `data3`, `text2` FROM `npc_exchanger` WHERE `type` = 4");
		Field *fields4 = type4->Fetch();
		if (pPlayer->GetArenaPoints() >= fields4[0].GetInt32())
		{
			pPlayer->ModifyArenaPoints((-1)*fields4[0].GetInt32());
			pPlayer->StoreNewItemInBestSlots(fields4[2].GetInt32(), fields4[1].GetInt32());
			pCreature->MonsterWhisper(fields4[3].GetString(), pPlayer->GetGUID());
		}
		else
			pCreature->MonsterWhisper("You have not enough Arena Points", pPlayer->GetGUID());
		}
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1005:
		{
		QueryResult* type5 = WorldDatabase.PQuery("SELECT `data1`, `data2`, `data3`, `text2` FROM `npc_exchanger` WHERE `type` = 5");
		Field *fields5 = type5->Fetch();
		if (pPlayer->HasItemCount(fields5[2].GetInt32(), fields5[0].GetInt32()))
		{
			pPlayer->DestroyItemCount(fields5[2].GetInt32(), fields5[0].GetInt32(), true);
			pPlayer->ModifyHonorPoints(fields5[1].GetInt32());
			pCreature->MonsterWhisper(fields5[3].GetString(), pPlayer->GetGUID());
		}
		else
			pCreature->MonsterWhisper("You have not enough Items", pPlayer->GetGUID());
		}
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1006:
		{
		QueryResult* type6 = WorldDatabase.PQuery("SELECT `data1`, `data2`, `data3`, `text2` FROM `npc_exchanger` WHERE `type` = 6");
		Field *fields6 = type6->Fetch();
		if (pPlayer->HasItemCount(fields6[2].GetInt32(), fields6[0].GetInt32()))
		{
			pPlayer->DestroyItemCount(fields6[2].GetInt32(), fields6[0].GetInt32(), true);
			pPlayer->ModifyArenaPoints(fields6[1].GetInt32());
			pCreature->MonsterWhisper(fields6[3].GetString(), pPlayer->GetGUID());
		}
		else
			pCreature->MonsterWhisper("You have not enough Items", pPlayer->GetGUID());
		}
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1007:
		{
		QueryResult* type7 = WorldDatabase.PQuery("SELECT `data1`, `data2`, `data3`, `text2` FROM `npc_exchanger` WHERE `type` = 7");
		Field *fields7 = type7->Fetch();
		if (pPlayer->GetMoney() >= fields7[0].GetInt32())
		{
			pPlayer->ModifyMoney((-1)*fields7[0].GetInt32());
			pPlayer->StoreNewItemInBestSlots(fields7[2].GetInt32(), fields7[1].GetInt32());
			pCreature->MonsterWhisper(fields7[3].GetString(), pPlayer->GetGUID());
		}
		else
			pCreature->MonsterWhisper("You have not enough Gold", pPlayer->GetGUID());
		}
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1008:
		{
		QueryResult* type8 = WorldDatabase.PQuery("SELECT `data1`, `data2`, `data3`, `text2` FROM `npc_exchanger` WHERE `type` = 8");
		Field *fields8 = type8->Fetch();
		if (pPlayer->HasItemCount(fields8[2].GetInt32(), fields8[0].GetInt32()))
		{
			pPlayer->DestroyItemCount(fields8[2].GetInt32(), fields8[0].GetInt32(), true);
			pPlayer->ModifyMoney(fields8[1].GetInt32());
			pCreature->MonsterWhisper(fields8[3].GetString(), pPlayer->GetGUID());
		}
		else
			pCreature->MonsterWhisper("You have not enough Items", pPlayer->GetGUID());
		}
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1009:
		{
		QueryResult* type9 = WorldDatabase.PQuery("SELECT `data1`, `data2`, `data3`, `text2` FROM `npc_exchanger` WHERE `type` = 9");
		Field *fields9 = type9->Fetch();
		if (pPlayer->GetMoney() >= fields9[0].GetInt32())
		{
			pPlayer->ModifyMoney((-1)*fields9[0].GetInt32());
			pPlayer->ModifyHonorPoints(fields9[1].GetInt32());
			pCreature->MonsterWhisper(fields9[3].GetString(), pPlayer->GetGUID());
		}
		else
			pCreature->MonsterWhisper("You have not enough Gold", pPlayer->GetGUID());
		}
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1010:
		{
		QueryResult* type10 = WorldDatabase.PQuery("SELECT `data1`, `data2`, `data3`, `text2` FROM `npc_exchanger` WHERE `type` = 10");
		Field *fields10 = type10->Fetch();
		if (pPlayer->GetHonorPoints() >= fields10[0].GetInt32())
		{
			pPlayer->ModifyHonorPoints((-1)*fields10[0].GetInt32());
			pPlayer->ModifyMoney(fields10[1].GetInt32());
			pCreature->MonsterWhisper(fields10[3].GetString(), pPlayer->GetGUID());
		}
		else
			pCreature->MonsterWhisper("You have not enough Honor Points", pPlayer->GetGUID());
		}
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1011:
		{
		QueryResult* type11 = WorldDatabase.PQuery("SELECT `data1`, `data2`, `data3`, `text2` FROM `npc_exchanger` WHERE `type` = 11");
		Field *fields11 = type11->Fetch();
		if (pPlayer->GetMoney() >= fields11[0].GetInt32())
		{
			pPlayer->ModifyMoney((-1)*fields11[0].GetInt32());
			pPlayer->ModifyArenaPoints(fields11[1].GetInt32());
			pCreature->MonsterWhisper(fields11[3].GetString(), pPlayer->GetGUID());
		}
		else
			pCreature->MonsterWhisper("You have not enough Gold", pPlayer->GetGUID());
		}
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1012:
		{
		QueryResult* type12 = WorldDatabase.PQuery("SELECT `data1`, `data2`, `data3`, `text2` FROM `npc_exchanger` WHERE `type` = 12");
		Field *fields12 = type12->Fetch();
		if (pPlayer->GetArenaPoints() >= fields12[0].GetInt32())
		{
			pPlayer->ModifyArenaPoints((-1)*fields12[0].GetInt32());
			pPlayer->ModifyMoney(fields12[1].GetInt32());
			pCreature->MonsterWhisper(fields12[3].GetString(), pPlayer->GetGUID());
		}
		else
			pCreature->MonsterWhisper("You have not enough Arena Points", pPlayer->GetGUID());
		}
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	}
}

bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    // Show menu
    if (uiSender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu(pPlayer, pCreature, uiAction);
    return true;
}
};

class npc_exchanger_Announce : public PlayerScript
{
public:

    npc_exchanger_Announce() : PlayerScript("npc_exchanger_Announce") {}

    void OnLogin(Player* player, bool firstlogin)
    {
        // Announce Module
        if (sWorld.GetModuleBoolConfig("NPC_Exchanger.Enable", true) && sWorld.GetModuleBoolConfig("NPC_Exchanger.Announce", true))
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00npc_exchanger |rmodule by |cff4CFF00LordPsyan");
        }
    }
};

void AddSC_npc_exchanger()
{
    new npc_exchanger();
}
