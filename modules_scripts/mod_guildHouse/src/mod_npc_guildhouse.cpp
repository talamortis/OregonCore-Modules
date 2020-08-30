#include "ScriptMgr.h"
#include "Configuration/Config.h"
#include "Player.h"
#include "World.h"
#include "Chat.h"
#include "Guild.h"
#include "mod_guildhouse.h"

class mod_npc_guildhouse : public CreatureScript
{
public:
    mod_npc_guildhouse() : CreatureScript("guildhouseNpc") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        Guild* guild = sObjectMgr.GetGuildById(player->GetGuildId());

        if (!guild)
            return false;

        if (guild->GetLeader() == player->GetGUID())
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Buy Guild House!", GOSSIP_SENDER_MAIN, 1);
            player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TABARD, "Sell Guild House!", GOSSIP_SENDER_MAIN, 2, "Are you sure you want to sell your Guild house?", NULL, false);
            if (!player->HasItemCount(9017, 1, true))
            {
                player->AddItem(9017, 1);
            }

        }
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Teleport to Guild House", GOSSIP_SENDER_MAIN, 3);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* m_creature, uint32 sender, uint32 action)
    {
        Guild* guild = sObjectMgr.GetGuildByLeader(player->GetGUID());

        switch (action)
        {
        case 1: // Select guild house
            sGuildHouse.SelectGuildHouse(guild, player, m_creature);
            break;
        case 2: // Sell Guild house
            sGuildHouse.SellGuildHouse(player, guild);
            break;
        case 3: // Teleport to GuildHouse
            sGuildHouse.TeleportToGuildHouse(guild, player, m_creature);
            break;
        }

        if (action >= 100)
        {
            sGuildHouse.BuyGuildHouse(player, guild, action);
        }
            

        return true;

    }
};

void Addmod_guildhouseScripts()
{
    new mod_npc_guildhouse();
}