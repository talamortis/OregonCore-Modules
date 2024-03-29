//Modified by Marcus rewritten by LordPsyan (https://www.psyanidegaming.com)
//original script created by ??
#include "ScriptPCH.h"
#include "Common.h"
#include "SystemConfig.h"
#include "World.h"

std::vector<uint32> HunterSpells = { 883, 982, 2641, 6991, 48990, 1002, 1462, 6197 };

#define GOSSIP_SENDER_MAIN      1000
#define MSG_TYPE                100004
#define MSG_PET                 100005
#define MAIN_MENU               "<= [Main Menu]"

class Npc_Beastmaster : public CreatureScript
{
public:
        Npc_Beastmaster() : CreatureScript("Npc_Beastmaster") { }


void CreatePet(Player *player, Creature * m_creature, uint32 entry) {

    if(!sWorld.GetModuleBoolConfig("BeastMaster.OnlyHunter", true)) // Checks to see if Only Hunters can have pets.
     {
        if(player->getClass() != CLASS_HUNTER) {
            m_creature->Whisper("You are not a Hunter!", LANG_UNIVERSAL, player);
            return;
        }
     }

    if(player->GetPet()) {
        m_creature->Whisper("First you must abandon your Pet!", LANG_UNIVERSAL, player);
        return;
    }

    Creature *creatureTarget = m_creature->SummonCreature(entry, player->GetPositionX(), player->GetPositionY()+2, player->GetPositionZ(), player->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 500);
    if(!creatureTarget) return;

    Pet* pet = player->CreateTamedPetFrom(creatureTarget, 0);

    if(!pet) return;

        // kill original creature
    creatureTarget->setDeathState(JUST_DIED);
    creatureTarget->RemoveCorpse();
    creatureTarget->SetHealth(0);                       // just for nice GM-mode view

    pet->SetPower(POWER_HAPPINESS, 1048000);

    // prepare visual effect for levelup
    pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel() - 1);
    pet->GetMap()->AddToMap((Creature*)pet);
        // visual effect for levelup
    pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel());

    if(!pet->InitStatsForLevel(player->getLevel()))
        sLog.outError("Pet Create fail: no init stats for entry %u", entry);
        pet->UpdateAllStats();

    // caster have pet now
        player->SetMinion(pet, true);

        pet->SavePetToDB(PET_SAVE_AS_CURRENT);
        //pet->InitTalentForLevel();
        player->PetSpellInitialize();
        if (!player->HasSpell(883))
        {
            for (int i = 0; i < HunterSpells.size(); ++i)
                player->LearnSpell(HunterSpells[i]);
        }
    //end
        player->CLOSE_GOSSIP_MENU();
        m_creature->Whisper("Pet added. You might want to feed it and name it somehow.", LANG_UNIVERSAL, player);
        return;
    }

bool OnGossipHello(Player* player, Creature* m_creature)
{
    bool EnableNormalPet = sWorld.GetModuleBoolConfig("BeastMaster.EnableNormalPet", true);

    // Main Menu

    // Check config if "Normal Pet " is enabled or not
    if(EnableNormalPet)
        player->ADD_GOSSIP_ITEM(  7, "|TInterface/ICONS/Ability_Hunter_Pet_Spider:30:30:-22|tNormal Pets ->"              , GOSSIP_SENDER_MAIN, 1000);
    // Now to add the spells, vendor, and stable stuffs
    player->ADD_GOSSIP_ITEM( 2, "|TInterface/ICONS/INV_Box_PetCarrier_01:25:25:-22|tPet Stable", GOSSIP_SENDER_MAIN, 6006);
    player->ADD_GOSSIP_ITEM( 6, "|TInterface/ICONS/INV_Misc_Petbiscuit_01:30:30:-22|tPet Food", GOSSIP_SENDER_MAIN, 6007);
    player->SEND_GOSSIP_MENU( MSG_TYPE, m_creature->GetGUID());
return true;
}

bool OnGossipSelect(Player* player, Creature* m_creature, uint32 uiSender, uint32 uiAction)
{
    // Main menu
    player->PlayerTalkClass->ClearMenus();
    if (uiSender == GOSSIP_SENDER_MAIN)
    SendDefaultMenu(player, m_creature, uiAction);

return true;
}

bool showNormalPet(Player *player, Creature *m_creature, uint32 showFromId = 0)
{
    QueryResult_AutoPtr result;
 result = WorldDatabase.PQuery("SELECT `entry`, `name` FROM `beastmaster` WHERE `cat_number` = 0 ORDER BY `entry` ASC");

 if (result)
 {
 uint32 entryNum = 0;
 std::string petName = "";
 uint8 tokenOrGold = 0;
 uint32 price = 0;
 uint32 nToken = 0;

  do
 {
 Field *fields = result->Fetch();
 entryNum = fields[0].GetInt32();
 petName = fields[1].GetString();

    player->ADD_GOSSIP_ITEM( 9, petName, GOSSIP_SENDER_MAIN, entryNum);
}
 while (result->NextRow());

    player->SEND_GOSSIP_MENU( MSG_PET, m_creature->GetGUID());
 return true;
 }
 else
 {
 if (showFromId == 0)
 {
 //you are too poor
 m_creature->Whisper("You don't have enough money.", LANG_UNIVERSAL, player);
 player->CLOSE_GOSSIP_MENU();
 }
 else
 {

 //show Spells from beginning
 showNormalPet(player, m_creature, 0);
 }
 }

 return false;
}

bool showPetSpells(Player *player, Creature *m_creature, uint32 showFromId = 0)
{
    QueryResult_AutoPtr result;
 result = WorldDatabase.PQuery("SELECT `entry`, `name` FROM `beastmaster` WHERE `cat_number` = 2 ORDER BY `entry` ASC");

 if (result)
 {
 uint32 entryNum = 0;
 std::string petName = "";
 uint8 tokenOrGold = 0;
 uint32 price = 0;
 uint32 nToken = 0;

  do
 {
 Field *fields = result->Fetch();
 entryNum = fields[0].GetInt32();
 petName = fields[1].GetString();

    player->ADD_GOSSIP_ITEM( 9, petName, GOSSIP_SENDER_MAIN, entryNum);
}
 while (result->NextRow());

    player->SEND_GOSSIP_MENU( MSG_PET, m_creature->GetGUID());
 return true;
 }
 else
 {
 if (showFromId == 0)
 {
 //you are too poor
 m_creature->Whisper("You don't have enough money.", player->GetGUID());
 player->CLOSE_GOSSIP_MENU();
 }
 else
 {

 //show Spells from beginning
 showPetSpells(player, m_creature, 0);
 }
 }

 return false;
}

void SendDefaultMenu(Player* player, Creature* m_creature, uint32 uiAction)
{

// Not allow in combat
if (player->IsInCombat())
{
    player->CLOSE_GOSSIP_MENU();
    m_creature->MonsterWhisper("You are in combat!", player->GetGUID());
    return;
}

    bool EnableNormalPet = sWorld.GetModuleBoolConfig("BeastMaster.EnableNormalPet", true);

  // send name as gossip item
    QueryResult_AutoPtr result;
        uint32 spellId = 0;
        uint32 catNumber = 0;
        uint32 cost = 0;
        std::string spellName = "";
        uint32 token = 0;
        bool tokenOrGold = true;

        result = WorldDatabase.PQuery("SELECT * FROM `beastmaster` WHERE `entry` = %u LIMIT 1", uiAction);

if (result)
{
    do {
        Field *fields = result->Fetch();
        catNumber = fields[1].GetInt32();
        tokenOrGold = fields[2].GetBool();
        cost = fields[3].GetInt32();
        token = fields[4].GetInt32();
        spellName = fields[5].GetString();
        spellId = fields[6].GetInt32();

        if (tokenOrGold)
        {
            if (!player->HasItemCount(token, cost))
                {
                    m_creature->Whisper("You ain't gots no darn chips.", LANG_UNIVERSAL, player);
                    player->CLOSE_GOSSIP_MENU();
                    return;
                }
            else if (uiAction != 1000 && catNumber != 2)
            {
    player->CLOSE_GOSSIP_MENU();
    CreatePet(player, m_creature, spellId);
    player->DestroyItemCount(token, cost, true);
            }
            else if (catNumber == 2)
            {
            if (player->HasSpell(spellId))
            {
                m_creature->Whisper("You already know this spell.", LANG_UNIVERSAL, player);
                player->CLOSE_GOSSIP_MENU();
                return;
            } else {
    player->CLOSE_GOSSIP_MENU();
    player->LearnSpell(spellId);
    player->DestroyItemCount(token, cost, true);
            }
        }

        } else {
            if (player->GetMoney() < cost)
            {
                m_creature->Whisper("You dont have enough money!", LANG_UNIVERSAL, player);
                player->CLOSE_GOSSIP_MENU();
                return;
            }
        else if (uiAction != 1000 && catNumber != 2)
        {
    player->CLOSE_GOSSIP_MENU();
    CreatePet(player, m_creature, spellId);
    player->ModifyMoney(-int(cost));
        }
        else if (catNumber == 2)
        {
            if (player->HasSpell(spellId))
            {
                m_creature->Whisper("You already know this spell.", LANG_UNIVERSAL, player);
                player->CLOSE_GOSSIP_MENU();
                return;
            } else {
    player->CLOSE_GOSSIP_MENU();
    player->LearnSpell(spellId);
    player->ModifyMoney(-int(cost));
            }
        }
    }
} while (result->NextRow());
} else {
//player->ADD_GOSSIP_ITEM(  7, MAIN_MENU, GOSSIP_SENDER_MAIN, 5005);
}

 switch(uiAction)
{

case 1000: //Normal Pet
        showNormalPet(player, m_creature, 0);
        //player->ADD_GOSSIP_ITEM(7, MAIN_MENU, GOSSIP_SENDER_MAIN, 5005);

    player->SEND_GOSSIP_MENU( MSG_PET, m_creature->GetGUID());
break;

    player->SEND_GOSSIP_MENU( MSG_PET, m_creature->GetGUID());
break;

case 5005: //Back To Main Menu
    // Main Menu
    // Check config if "Normal Pet " is enabled or not
    if(EnableNormalPet)
        player->ADD_GOSSIP_ITEM(  7, "|TInterface/ICONS/Ability_Hunter_Pet_Spider:30:30:-22|tNormal Pets ->"              , GOSSIP_SENDER_MAIN, 1000);
    // Now to add the spells, vendor, and stable stuffs
    player->ADD_GOSSIP_ITEM( 2, "|TInterface/ICONS/INV_Box_PetCarrier_01:30:30:-22|tPet Stable", GOSSIP_SENDER_MAIN, 6006);
    player->ADD_GOSSIP_ITEM( 6, "|TInterface/ICONS/INV_Misc_Petbiscuit_01:30:30:-22|tPet Food", GOSSIP_SENDER_MAIN, 6007);
    player->SEND_GOSSIP_MENU( MSG_TYPE, m_creature->GetGUID());
break;

case 6006:
    player->GetSession()->SendStablePet(m_creature->GetGUID());
    player->CLOSE_GOSSIP_MENU();
    break;

case 6007:
    player->GetSession()->SendListInventory(m_creature->GetGUID());
    player->CLOSE_GOSSIP_MENU();
    break;

/*case 6008:
    player->ResetPetTalents();
    player->CLOSE_GOSSIP_MENU();
    break;*/

 player->CLOSE_GOSSIP_MENU();
 }

} //end of function

};

class BeastMaster_Announce : public PlayerScript
{
public:

    BeastMaster_Announce() : PlayerScript("beastmaster_Announce") {}

    void OnLogin(Player* player, bool firstlogin)
    {
        // Announce Module
        if (sWorld.GetModuleBoolConfig("BeastMaster.EnableNormalPet", true) && sWorld.GetModuleBoolConfig("BeastMaster.Announce", true))
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00BeastMaster |rmodule by |cff4CFF00LordPsyan");
        }
    }
};

void AddSC_Npc_Beastmaster()
{
    new Npc_Beastmaster();
    new BeastMaster_Announce();
}
