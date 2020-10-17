#include "Player.h"
#include "mod_guildhouse.h"
#include "Item.h"
#include "Guild.h"
#include "World.h"
#include "Chat.h"
#include "Language.h"

class guildhouse_Item : public ItemScript
{
public:
    guildhouse_Item() : ItemScript("guildhouse") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        if (player->IsInCombat() || player->GetMap()->IsBattlegroundOrArena())
            return false;

        Guild* guild = sObjectMgr.GetGuildById(player->GetGuildId());

        if (!guild)
            return false;

        player->PlayerTalkClass->ClearMenus();

        if (guild->GetLeader() == player->GetGUID())
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ----- Management ----", GOSSIP_SENDER_MAIN, 2);
        }
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
        return false;
    }

    void OnGossipSelect(Player* player, Item* item, uint32 /*sender*/, uint32 action)
    {
        Guild* guild = sObjectMgr.GetGuildByLeader(player->GetGUID());

        switch (action)
        {
        case 2:
            sGuildHouse.ShowMainMenu(player, item);
            break;
        case 3:
            sGuildHouse.ShowGameObjectMenu(player, item);
            break;
        case 4: // Creatures
            sGuildHouse.ShowCreatureMainMenu(player, item);
            break;
        case 5:
            sGuildHouse.DeleteCreature(player);
            sGuildHouse.ShowMainMenu(player, item);
            break;
        case 7:
            sGuildHouse.OnListNearObjects(player, item);
            break;
        case 8:
            sGuildHouse.ShowGameObjectPortals(player, item);
            break;
        case 9: // Profession Trainers
            sGuildHouse.ShowProfessionTrainer(player, item);
            break;
        case 10: // Class Trainers
            sGuildHouse.ShowClassTrainers(player, item);
            break;
        case 11: // Misc (Innkeeper ect)
            sGuildHouse.ShowMiscMenu(player, item);
            break;
        /* Misc */
        case 18907:
        case 19573:
        case 19575:
        case 19213:
        case 19537:
        case 21733:
        case 15686:
        case 8670:
        case 18911:
        case 11188:
            sGuildHouse.SpawnNPC(action, player, sWorld.GetModuleIntConfig("GuildHouse.Misc", 500 * GOLD));
            sGuildHouse.ShowMiscMenu(player, item);
            break;
         /* Class Trainers*/
        case 26325:
        case 26326:
        case 26327:
        case 26328:
        case 26329:
        case 26330:
        case 26331:
        case 26332:
        case 26324:
            sGuildHouse.SpawnNPC(action, player, sWorld.GetModuleIntConfig("GuildHouse.Trainer", 500 * GOLD));
            sGuildHouse.ShowClassTrainers(player, item);
            break;
        /* Profession Trainers*/
        case 16823:
        case 16583:
        case 19052:
        case 19540:
        case 19187:
        case 19180:
        case 18772:
        case 18749:
        case 24868:
        case 18779:
        case 18776:
        case 18748:
        case 22477:
        case 4210:
        case 4552:
        case 3607:
        case 3332:
            sGuildHouse.SpawnNPC(action, player, sWorld.GetModuleIntConfig("GuildHouse.ProfTrainer", 500 * GOLD));
            sGuildHouse.ShowProfessionTrainer(player, item);
            break;
        /* Objects */
        case 183325: // Stormwind Portal
        case 183323: // Orgrimmar Portal
        case 183322: // Ironforge Portal
        case 183327: // Undercity Portal
        case 183317: // Darnassus Portal
        case 183326: // Thunder bluff portal
        case 183324: // Silvermoon Portal
        case 183321: // Exodar Portal
            sGuildHouse.SpawnObject(action, player, sWorld.GetModuleIntConfig("GuildHouse.Portal", 500 * GOLD));
            sGuildHouse.ShowGameObjectPortals(player, item);
            break; 
        case 184137: // Mailbox
            sGuildHouse.SpawnObject(action, player, sWorld.GetModuleIntConfig("GuildHouse.Mailbox", 500 * GOLD));
            sGuildHouse.ShowGameObjectMenu(player, item);
            break;
        case 24469: // Chair
            sGuildHouse.SpawnObject(action, player, sWorld.GetModuleIntConfig("GuildHouse.Mailbox", 500 * GOLD));
            sGuildHouse.ShowGameObjectMenu(player, item);
            break;
        case 187334: // Guild Vault
            sGuildHouse.SpawnObject(action, player, sWorld.GetModuleIntConfig("GuildHouse.GuildVault", 500 * GOLD));
            sGuildHouse.ShowGameObjectMenu(player, item);
            break;
        /* Misc Objects*/
        case 1685:
        case 38492:
        case 183848:
            sGuildHouse.SpawnObject(action, player, sWorld.GetModuleIntConfig("GuildHouse.ObjectMisc", 500 * GOLD));
            sGuildHouse.ShowGameObjectMenu(player, item);
            break;
        case 60000:
            sGuildHouse.ShowMainMenu(player, item);
            break;
        default:
            break;
        }
    }

    void OnGossipSelectCode(Player* player, Item* item, uint32 sender, uint32 action, const char* code) override
    {

        QueryResult* result = CharacterDatabase.PQuery("SELECT `id`, `guild`, `phase`, `map`,`positionX`, `positionY`, `positionZ`, `zoneId` FROM guild_house WHERE `guild` = %u", player->GetGuildId());;

        if (!result)
            return;

        Field* fields = result->Fetch();

        //check we are the right zone before allowing to spawn
        if (player->GetZoneId() != fields[7].GetUInt32())
        {
            ChatHandler(player).PSendSysMessage("You can only remove Objects in your Guildhouse!");
            return;
        }

        if (player->GetPhaseMask() != fields[2].GetUInt32())
        {
            ChatHandler(player).PSendSysMessage("You can only remove Objects in your guildHouse Phase!");
            return;
        }

        if (!code)
            return;

        uint32 lowguid = atoi(code);

        if (!lowguid)
            return;

        GameObject* obj = NULL;
        ChatHandler handler(player->GetSession());


        // by DB guid
        if (GameObjectData const* go_data = sObjectMgr.GetGOData(lowguid))
            obj = handler.GetObjectGlobalyWithGuidOrNearWithDbGuid(lowguid, go_data->id);

        if (!obj)
        {
            handler.PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, lowguid);
            handler.SetSentErrorMessage(true);
            return;
        }

        uint64 owner_guid = obj->GetOwnerGUID();
        if (owner_guid)
        {
            Unit* owner = ObjectAccessor::GetUnit(*handler.GetSession()->GetPlayer(), owner_guid);
            if (!owner || !IS_PLAYER_GUID(owner_guid))
            {
                handler.PSendSysMessage(LANG_COMMAND_DELOBJREFERCREATURE, GUID_LOPART(owner_guid), obj->GetGUIDLow());
                handler.SetSentErrorMessage(true);
                return;
            }

            owner->RemoveGameObject(obj, false);
        }

        obj->SetRespawnTime(0);                                 // not save respawn time
        obj->Delete();
        obj->DeleteFromDB();

        handler.PSendSysMessage(LANG_COMMAND_DELOBJMESSAGE, obj->GetGUIDLow());
        sGuildHouse.ShowMainMenu(player, item);
    }
};

class GuildHousePlayer : public PlayerScript
{
public:
    GuildHousePlayer() : PlayerScript("GuildHousePlayer") {}

    void OnUpdateZone(Player* player, uint32 newZone, uint32 newArea)
    {
        if (newArea == 876)
            player->SetPhaseMask(player->GetGuildId(), true, true);
        else
            player->RestorePhase();
    }
};

class GuildHouseGuild : public GuildScript
{
public:
    GuildHouseGuild() : GuildScript("GuildHouse") {}

    void OnDisband(Guild* guild)
    {
        QueryResult* result = CharacterDatabase.PQuery("SELECT `id`, `map` FROM guild_house WHERE `guild` = %u", guild->GetId());

        if (!result)
            return;

        Field* fields = result->Fetch();

        if (guild->GetId() != 1)
        {
            WorldDatabase.PQuery("DELETE FROM `creature` WHERE map = %u AND phaseMask = %u", fields[1].GetUInt32(), guild->GetId());
            WorldDatabase.PQuery("DELETE FROM `gameobject` WHERE map = %u and phaseMask = %u", fields[1].GetUInt32(), guild->GetId());
        }

        CharacterDatabase.PQuery("DELETE FROM `guild_house` WHERE guild = %u", guild->GetId());
    }

};

void Addmod_guildhouseItemScripts()
{
    new GuildHouseGuild();
    new guildhouse_Item();
    new GuildHousePlayer();
}