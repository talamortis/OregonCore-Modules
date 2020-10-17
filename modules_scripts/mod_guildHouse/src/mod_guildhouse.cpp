#include "Guild.h"
#include "Player.h"
#include "Map.h"
#include "mod_guildhouse.h"
#include "Language.h"

bool GuildHouse::SelectGuildHouse(Guild* guild, Player* player, Creature* creature)
{
    QueryResult* result = CharacterDatabase.PQuery("SELECT `id`, `guild` FROM guild_house WHERE `guild` = %u", player->GetGuildId());

    if (result)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("You cant buy any more guilds houses!");
        player->CLOSE_GOSSIP_MENU();
        return false;
    }

    player->PlayerTalkClass->ClearMenus();
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "GM Island", GOSSIP_SENDER_MAIN, 100, "Buy GM Island Guildhouse", sWorld.GetModuleIntConfig("GuildHouse.Cost", 10000000), false);
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ----- More to Come ----", GOSSIP_SENDER_MAIN, 4);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    return true;
}

bool GuildHouse::SellGuildHouse(Player* player, Guild* guild)
{

    QueryResult* result = CharacterDatabase.PQuery("SELECT id, `guild` FROM `guild_house` WHERE guild = %u", player->GetGuildId());

    if (!result)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("You do not have a active Guild house!");
        return false;
    }

    CharacterDatabase.PQuery("DELETE FROM `guild_house` WHERE guild = %u", player->GetGuildId());

    if (player->GetGuildId() != 1)
    {
        WorldDatabase.PQuery("DELETE FROM `creature` WHERE `map` = 1 AND phaseMask = %u", player->GetGuildId());
        WorldDatabase.PQuery("DELETE FROM `gameobject` WHERE `map` = 1 and phaseMask = %u", player->GetGuildId());
    }

    ChatHandler(player->GetSession()).PSendSysMessage("You have successfully sold your guild house");
    player->ModifyMoney(+(sWorld.GetModuleIntConfig("GuildHouse.Cost", 10000000) / 2));
    player->CLOSE_GOSSIP_MENU();
    return true;
}

bool GuildHouse::BuyGuildHouse(Player* player, Guild* guild, uint32 action)
{
    switch (action)
    {
    case 100:
        map = 1;
        posX = 16226.117f;
        posY = 16258.046f;
        posZ = 13.257628f;
        zoneId = 876;
        break;
    }

    CharacterDatabase.PQuery("INSERT INTO `guild_house` (guild, phase, map, positionX, positionY, positionZ, zoneId) VALUES (%u, %u, %u, %f, %f, %f, %u)", player->GetGuildId(), player->GetGuildId(), map, posX, posY, posZ, zoneId);
    player->ModifyMoney(-(sWorld.GetModuleIntConfig("GuildHouse.Cost", 10000000)));
    ChatHandler(player->GetSession()).PSendSysMessage("You have successfully purchased a guild house");
    guild->BroadcastToGuild(player->GetSession(), "We have now got a guild house", LANG_UNIVERSAL);
    player->SaveGoldToDB(); // Save players gold just incase crash
    player->CLOSE_GOSSIP_MENU();
    return true;
}

void GuildHouse::TeleportToGuildHouse(Guild* guild, Player* player, Creature* creature)
{

    if (player->IsInCombat() || player->IsInFlight())
    {
        ChatHandler(player).PSendSysMessage("Unable to teleport to GuildHouse");
        return;
    }

    QueryResult* result = CharacterDatabase.PQuery("SELECT `id`, `guild`, `phase`, `map`,`positionX`, `positionY`, `positionZ` FROM guild_house WHERE `guild` = %u", player->GetGuildId());

    if (!result)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("Your Guild does not own a guild house");
        return;
    }

    Field* fields = result->Fetch();
    player->TeleportTo(fields[3].GetUInt32(), fields[4].GetFloat(), fields[5].GetFloat(), fields[6].GetFloat(), player->GetOrientation());
}

void GuildHouse::SpawnNPC(uint32 entry, Player* player, uint32 cost)
{
    QueryResult* result = CharacterDatabase.PQuery("SELECT `id`, `guild`, `phase`, `map`,`positionX`, `positionY`, `positionZ`, `zoneId` FROM guild_house WHERE `guild` = %u", player->GetGuildId());;

    if (!result)
        return;

    Field* fields = result->Fetch();

    //check we are the right zone before allowing to spawn
    if (player->GetZoneId() != fields[7].GetUInt32())
    {
        ChatHandler(player).PSendSysMessage("Creatures can only be spawned in your Guildhouse!");
        return;
    }

    if (player->FindNearestCreature(entry, VISIBILITY_RANGE))
    {
        ChatHandler(player).PSendSysMessage("This creature is already spawned.");
        return;
    }

    // Now make sure we are in the right phase
    if (fields[2].GetUInt32() != player->GetGuildId())
    {
        ChatHandler(player).PSendSysMessage("Incorrect Phasing to spawn creature");
        return;
    }

    Creature* creature = new Creature;
    if (!creature->Create(sObjectMgr.GenerateLowGuid(HIGHGUID_UNIT), player->GetMap(), player->GetPhaseMask(), entry, (uint32)0, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation()))
    {
        delete creature;
        return;
    }

    creature->SaveToDB(player->GetMap()->GetId(), (1 << player->GetMap()->GetSpawnMode()), player->GetGuildId());

    uint32 db_guid = creature->GetDBTableGUIDLow();

    // To call _LoadGoods(); _LoadQuests(); CreateTrainerSpells();
    if (!creature->LoadCreatureFromDB(db_guid, player->GetMap()))
    {
        delete creature;
        return;
    }

    sObjectMgr.AddCreatureToGrid(db_guid, sObjectMgr.GetCreatureData(db_guid));
    player->ModifyMoney(-cost);
    player->SaveGoldToDB();
    player->CLOSE_GOSSIP_MENU();
}

void GuildHouse::SpawnObject(uint32 entry, Player* player, uint32 cost)
{
    QueryResult* result = CharacterDatabase.PQuery("SELECT `id`, `guild`, `phase`, `map`,`positionX`, `positionY`, `positionZ`, `zoneId` FROM guild_house WHERE `guild` = %u", player->GetGuildId());;

    if (!result)
        return;

    Field* fields = result->Fetch();

    //check we are the right zone before allowing to spawn
    if (player->GetZoneId() != fields[7].GetUInt32())
    {
        ChatHandler(player).PSendSysMessage("Objects can only be spawned in your Guildhouse!");
        return;
    }

    // Now make sure we are in the right phase
    if (fields[2].GetUInt32() != player->GetGuildId())
    {
        ChatHandler(player).PSendSysMessage("Incorrect Phasing to spawn creature");
        return;
    }

    if (player->FindNearestGameObject(entry, VISIBLE_RANGE) && entry != 24469)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("You already have this object!");
        player->CLOSE_GOSSIP_MENU();
        return;
    }

    uint32 objectId = entry;
    if (!objectId)
        return;

    const GameObjectInfo* gInfo = sObjectMgr.GetGameObjectInfo(objectId);

    if (!gInfo)
        return;

    if (gInfo->displayId && !sGameObjectDisplayInfoStore.LookupEntry(gInfo->displayId))
        return;

    GameObject* object = new GameObject;
    uint32 guidLow = sObjectMgr.GenerateLowGuid(HIGHGUID_GAMEOBJECT);

    if (!object->Create(guidLow, gInfo->id, player->GetMap(), player->GetPhaseMask(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 0, GO_STATE_READY))
    {
        delete object;
        return;
    }

    // fill the gameobject data and save to the db
    object->SaveToDB(player->GetMapId(), (1 << player->GetMap()->GetSpawnMode()), player->GetGuildId());
    // delete the old object and do a clean load from DB with a fresh new GameObject instance.
    // this is required to avoid weird behavior and memory leaks
    delete object;

    object = new GameObject;
    // this will generate a new guid if the object is in an instance
    if (!object->LoadGameObjectFromDB(guidLow, player->GetMap()))
    {
        delete object;
        return;
    }

    // TODO: is it really necessary to add both the real and DB table guid here ?
    sObjectMgr.AddGameobjectToGrid(guidLow, sObjectMgr.GetGOData(guidLow));
    player->ModifyMoney(-cost);
    player->SaveGoldToDB();
    player->CLOSE_GOSSIP_MENU();

}

void GuildHouse::DeleteCreature(Player* player)
{
    Creature* unit = player->GetSelectedUnit()->ToCreature();

    if (!unit)
        return;

    QueryResult* result = CharacterDatabase.PQuery("SELECT `id`, `guild`, `phase`, `map`,`positionX`, `positionY`, `positionZ`, `zoneId` FROM guild_house WHERE `guild` = %u", player->GetGuildId());;

    if (!result)
        return;

    Field* fields = result->Fetch();

    //check we are the right zone before allowing to spawn
    if (player->GetZoneId() != fields[7].GetUInt32())
    {
        ChatHandler(player).PSendSysMessage("You can only remove creatures in your Guildhouse!");
        return;
    }

    // Delete creature
    unit->CombatStop();
    unit->DeleteFromDB();
    unit->AddObjectToRemoveList();
    ChatHandler(player).SendSysMessage(LANG_COMMAND_DELCREATMESSAGE);
}

bool GuildHouse::ShowGameObjectPortals(Player* player, Item* item)
{
    player->PlayerTalkClass->ClearMenus();
    if (player->GetTeamId() == TEAM_ALLIANCE)
    {
        player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TAXI, "Portal: Stormwind", GOSSIP_SENDER_MAIN, 183325, "Add Stormwind Portal?", sWorld.GetModuleIntConfig("GuildHouse.Portal", 500*GOLD), false);
         player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TAXI, "Portal: Ironforge", GOSSIP_SENDER_MAIN, 183322, "Add Ironforge Portal?", sWorld.GetModuleIntConfig("GuildHouse.Portal", 500 * GOLD), false);
         player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TAXI, "Portal: Darnassus", GOSSIP_SENDER_MAIN, 183317, "Add Darnassus Portal?", sWorld.GetModuleIntConfig("GuildHouse.Portal", 500 * GOLD), false);
         player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TAXI, "Portal: Exodar", GOSSIP_SENDER_MAIN, 183321, "Add Exodar Portal?", sWorld.GetModuleIntConfig("GuildHouse.Portal", 500 * GOLD), false);
    }
    else
    {
        player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TAXI, "Portal: Orgrimmar", GOSSIP_SENDER_MAIN, 183323, "Add Orgrimmar Portal?", sWorld.GetModuleIntConfig("GuildHouse.Portal", 500 * GOLD), false);
        player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TAXI, "Portal: Undercity", GOSSIP_SENDER_MAIN, 183327, "Add Undercity Portal?", sWorld.GetModuleIntConfig("GuildHouse.Portal", 500 * GOLD), false);
        player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TAXI, "Portal: Thunderbluff", GOSSIP_SENDER_MAIN, 183326, "Add Thunderbuff Portal?", sWorld.GetModuleIntConfig("GuildHouse.Portal", 500 * GOLD), false);
        player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TAXI, "Portal: Silvermoon", GOSSIP_SENDER_MAIN, 183324, "Add Silvermoon Portal?", sWorld.GetModuleIntConfig("GuildHouse.Portal", 500 * GOLD), false);
    }
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Main Menu!", GOSSIP_SENDER_MAIN, 60000);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
    return true;
}

bool GuildHouse::ShowGameObjectMenu(Player* player, Item* item)
{
    player->PlayerTalkClass->ClearMenus();
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Portals", GOSSIP_SENDER_MAIN, 8);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "Guild Vault", GOSSIP_SENDER_MAIN, 187334, "Add a Guild Vault?", sWorld.GetModuleIntConfig("GuildHouse.GuildVault", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TALK, "Mailbox", GOSSIP_SENDER_MAIN, 184137, "Add a mailbox?", sWorld.GetModuleIntConfig("GuildHouse.MailBox", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Chair", GOSSIP_SENDER_MAIN, 24469, "Add a Chair?", sWorld.GetModuleIntConfig("GuildHouse.Chair", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Anvil", GOSSIP_SENDER_MAIN, 38492, "Add a Anvil?", sWorld.GetModuleIntConfig("GuildHouse.ObjectMisc", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Forge", GOSSIP_SENDER_MAIN, 1685, "Add a Forge?", sWorld.GetModuleIntConfig("GuildHouse.ObjectMisc", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, "Alchemy Lab", GOSSIP_SENDER_MAIN, 183848, "Add a Alchemy Lab?", sWorld.GetModuleIntConfig("GuildHouse.ObjectMisc", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Main Menu!", GOSSIP_SENDER_MAIN, 60000);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
    return true;
}

bool GuildHouse::ShowCreatureMainMenu(Player* player, Item* item)
{
    player->PlayerTalkClass->ClearMenus();
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Profession Trainers", GOSSIP_SENDER_MAIN, 9);
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "Class Trainers", GOSSIP_SENDER_MAIN, 10);
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Misc", GOSSIP_SENDER_MAIN, 11);
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Main Menu!", GOSSIP_SENDER_MAIN, 60000);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
    return true;
}

bool GuildHouse::ShowClassTrainers(Player* player, Item* item)
{
    player->PlayerTalkClass->ClearMenus();
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Druid", GOSSIP_SENDER_MAIN, 26324, "Spawn Druid Trainer?", sWorld.GetModuleIntConfig("GuildHouse.Trainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Hunter", GOSSIP_SENDER_MAIN, 26325, "Spawn Hunter Trainer?", sWorld.GetModuleIntConfig("GuildHouse.Trainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Mage", GOSSIP_SENDER_MAIN, 26326, "Spawn Mage Trainer?", sWorld.GetModuleIntConfig("GuildHouse.Trainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Paladin", GOSSIP_SENDER_MAIN, 26327, "Spawn Paladin Trainer?", sWorld.GetModuleIntConfig("GuildHouse.Trainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Priest", GOSSIP_SENDER_MAIN, 26328, "Spawn Priest Trainer?", sWorld.GetModuleIntConfig("GuildHouse.Trainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Rogue", GOSSIP_SENDER_MAIN, 26329, "Spawn Rogue Trainer?", sWorld.GetModuleIntConfig("GuildHouse.Trainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Shaman", GOSSIP_SENDER_MAIN, 26330, "Spawn Shaman Trainer?", sWorld.GetModuleIntConfig("GuildHouse.Trainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Warlock", GOSSIP_SENDER_MAIN, 26331, "Spawn Warlock Trainer?", sWorld.GetModuleIntConfig("GuildHouse.Trainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Warrior", GOSSIP_SENDER_MAIN, 26332, "Spawn Warrior Trainer?", sWorld.GetModuleIntConfig("GuildHouse.Trainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Main Menu", GOSSIP_SENDER_MAIN, 60000);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
    return true;
}

bool GuildHouse::ShowMainMenu(Player* player, Item* item)
{
    player->PlayerTalkClass->ClearMenus();
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Game Objects", GOSSIP_SENDER_MAIN, 3);
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "Creatures", GOSSIP_SENDER_MAIN, 4);
    player->ADD_GOSSIP_ITEM(GOSSIP_ACTION_BATTLE, "Remove Creature", GOSSIP_SENDER_MAIN, 5);
    player->ADD_GOSSIP_ITEM(GOSSIP_ACTION_BATTLE, "Remove Object", GOSSIP_SENDER_MAIN, 7);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
    return true;
}

bool GuildHouse::ShowProfessionTrainer(Player* player, Item* item)
{
    player->PlayerTalkClass->ClearMenus();
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Herbalism Trainer", GOSSIP_SENDER_MAIN, player->GetTeamId() == TEAM_ALLIANCE ? 18776 : 18748, "Spawn Engineering Trainer?", sWorld.GetModuleIntConfig("GuildHouse.ProfTrainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Mining Trainer", GOSSIP_SENDER_MAIN, player->GetTeamId() == TEAM_ALLIANCE ? 18779 : 18747, "Spawn Engineering Trainer?", sWorld.GetModuleIntConfig("GuildHouse.ProfTrainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Tailoring Trainer", GOSSIP_SENDER_MAIN, player->GetTeamId() == TEAM_ALLIANCE ? 18772 : 16583, "Spawn Skinning Trainer?", sWorld.GetModuleIntConfig("GuildHouse.ProfTrainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Blacksmithing Trainer", GOSSIP_SENDER_MAIN, player->GetTeamId() == TEAM_ALLIANCE ? 16823 : 16583, "Spawn Blacksmithing Trainer?", sWorld.GetModuleIntConfig("GuildHouse.ProfTrainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Alchemy Trainer", GOSSIP_SENDER_MAIN, 19052, "Spawn Alchemy Trainer?", sWorld.GetModuleIntConfig("GuildHouse.ProfTrainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Enchanting Trainer", GOSSIP_SENDER_MAIN, 19540, "Spawn Enchanting Trainer?", sWorld.GetModuleIntConfig("GuildHouse.ProfTrainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Jewelcrafting Trainer", GOSSIP_SENDER_MAIN, 19539, "Spawn Jewelcrafting Trainer?", sWorld.GetModuleIntConfig("GuildHouse.ProfTrainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Leatherworking Trainer", GOSSIP_SENDER_MAIN, 19187, "Spawn Leatherworking Trainer?", sWorld.GetModuleIntConfig("GuildHouse.ProfTrainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Skinning Trainer", GOSSIP_SENDER_MAIN, 19180, "Spawn Skinning Trainer?", sWorld.GetModuleIntConfig("GuildHouse.ProfTrainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Engineering Trainer", GOSSIP_SENDER_MAIN, 24868, "Spawn Engineering Trainer?", sWorld.GetModuleIntConfig("GuildHouse.ProfTrainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Cooking Trainer", GOSSIP_SENDER_MAIN, player->GetTeamId() == TEAM_ALLIANCE ? 4210 : 4552, "Spawn Cooking Trainer?", sWorld.GetModuleIntConfig("GuildHouse.ProfTrainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Fist Aid Trainer", GOSSIP_SENDER_MAIN, 22477, "Spawn First Aid Trainer?", sWorld.GetModuleIntConfig("GuildHouse.ProfTrainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "Fishing Trainer", GOSSIP_SENDER_MAIN, player->GetTeamId() == TEAM_ALLIANCE ? 3607 : 3332, "Spawn Cooking Trainer?", sWorld.GetModuleIntConfig("GuildHouse.ProfTrainer", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Main Menu", GOSSIP_SENDER_MAIN, 60000);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());

    return true;
}

bool GuildHouse::ShowMiscMenu(Player* player, Item* item)
{
    player->PlayerTalkClass->ClearMenus();
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "Innkeeper", GOSSIP_SENDER_MAIN, 18907, "Spawn InnKeeper?", sWorld.GetModuleIntConfig("GuildHouse.Misc", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, "Banker", GOSSIP_SENDER_MAIN, 21733, "Spawn Banker?", sWorld.GetModuleIntConfig("GuildHouse.Misc", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TALK, "Auctioneer", GOSSIP_SENDER_MAIN, player->GetTeamId() == TEAM_ALLIANCE ? 8670 : 15686, "Spawn Auctioneer?", sWorld.GetModuleIntConfig("GuildHouse.Misc", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_VENDOR, "Trade Supplies", GOSSIP_SENDER_MAIN, 19573, "Spawn Trade Supplies?", sWorld.GetModuleIntConfig("GuildHouse.Misc", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_VENDOR, "Alchemy Supplies", GOSSIP_SENDER_MAIN, 11188, "Spawn Alchemy Supplies?", sWorld.GetModuleIntConfig("GuildHouse.Misc", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_VENDOR, "Tailoring Supplies", GOSSIP_SENDER_MAIN, 19213, "Spawn Tailoring Supplies?", sWorld.GetModuleIntConfig("GuildHouse.Misc", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_VENDOR, "Engineering Supplies", GOSSIP_SENDER_MAIN, 19575, "Spawn Engineering Supplies?", sWorld.GetModuleIntConfig("GuildHouse.Misc", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_VENDOR, "Enchanting Supplies", GOSSIP_SENDER_MAIN, 19537, "Spawn Enchanting Supplies?", sWorld.GetModuleIntConfig("GuildHouse.Misc", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_VENDOR, "Cooking Supplies", GOSSIP_SENDER_MAIN, 19195, "Spawn Cooking Supplies?", sWorld.GetModuleIntConfig("GuildHouse.Misc", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_VENDOR, "Fishing Supplies", GOSSIP_SENDER_MAIN, 18911, "Spawn Fishing Supplies?", sWorld.GetModuleIntConfig("GuildHouse.Misc", 500 * GOLD), false);
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Main Menu", GOSSIP_SENDER_MAIN, 60000);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
    return true;
}

bool GuildHouse::OnListNearObjects(Player* player, Item* item)
{
    float distance =  10.0f;
    uint32 count = 0;

    QueryResult* result = WorldDatabase.PQuery("SELECT guid, id, position_x, position_y, position_z, map, "
        "(POW(position_x - '%f', 2) + POW(position_y - '%f', 2) + POW(position_z - '%f', 2)) AS order_ "
        "FROM gameobject WHERE phaseMask='%u' AND map='%u'  AND (POW(position_x - '%f', 2) + POW(position_y - '%f', 2) + POW(position_z - '%f', 2)) <= '%f' ORDER BY order_",
        player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetGuildId(),
        player->GetMapId(), player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), distance * distance);

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 guid = fields[0].GetUInt32();
            uint32 entry = fields[1].GetUInt32();
            float x = fields[2].GetFloat();
            float y = fields[3].GetFloat();
            float z = fields[4].GetFloat();
            int mapid = fields[6].GetUInt16();

            GameObjectInfo const* gInfo = sObjectMgr.GetGameObjectInfo(entry);

            if (!gInfo)
                continue;

           ChatHandler(player).PSendSysMessage(LANG_GO_LIST_CHAT, guid, entry, guid, gInfo->name, x, y, z, mapid);

            ++count;
        } while (result->NextRow());
    }

    ChatHandler(player).PSendSysMessage(LANG_COMMAND_NEAROBJMESSAGE, distance, count);
    player->PlayerTalkClass->ClearMenus();
    player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "Remove GameObjet.", GOSSIP_SENDER_MAIN, 7, "Please enter the GUID", 0, true);
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Main Menu", GOSSIP_SENDER_MAIN, 60000);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
    return true;
}