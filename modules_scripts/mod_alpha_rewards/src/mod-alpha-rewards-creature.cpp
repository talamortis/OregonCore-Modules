#include "mod-alpha-rewards.h"
#include "Creature.h"
#include "Player.h"
#include "GossipDef.h"
#include "ScriptedGossip.h"
#include "Item.h"
#include "AccountMgr.h"
#include "ScriptMgr.h"
#include <unordered_map>

const uint32 items_on_page = 32 - 2; // max option count - next and previous buttons

class alpha_reward_system_npc : public CreatureScript
{
public:
    alpha_reward_system_npc() : CreatureScript("alpha_reward_system_npc") { }


    void ShowPage(Player* player, Creature* creature, uint32 page)
    {
        uint32 start = page * items_on_page;
        uint32 end = std::min<uint32>((page + 1) * items_on_page, sAlphaRewards->id.size());

        std::string Itemcolor;
        uint32 width = 30;
        uint32 height = 30;
        int x = -20;
        int y = 0;
        for (uint32 i = start; i < end; ++i)
        {
            switch (sObjectMgr.GetItemTemplate(sAlphaRewards->itemId[i])->Quality)
            {
            case 0: // poor
                Itemcolor = "|cFF808080";
                break;
            case 1: // Common
                Itemcolor = "|cffffffff";
                break;
            case 2: // uncommon
                Itemcolor = "|cFF008000";
                break;
            case 3: // rare
                Itemcolor = "|cFF0000FF";
                break;
            case 4://epic
                Itemcolor = "|cFF800080";
                break;
            case 5: //Legendary
                Itemcolor = "|cFFFF4500";
                break;
            default:
                Itemcolor = "";
                break;
            }

            std::ostringstream ss;
            std::ostringstream confirm;
            std::ostringstream points;
            std::ostringstream itempoints;
            ss << Itemcolor.c_str() << "|TInterface";
            const ItemTemplate* temp = sObjectMgr.GetItemTemplate(sAlphaRewards->itemId[i]);
            const ItemDisplayInfoEntry* dispInfo = NULL;

            if (temp)
            {
                dispInfo = sItemDisplayInfoStore.LookupEntry(temp->DisplayInfoID);
                if (dispInfo)
                    ss << "/ICONS/" << dispInfo->inventoryIcon;
            }
            if (!dispInfo)
                ss << "/InventoryItems/WoWUnknownItem01";
            ss << ":" << width << ":" << height << ":" << x << ":" << y << "|t" << sObjectMgr.GetItemTemplate(sAlphaRewards->itemId[i])->Name1 << "|r";
            confirm << "Are you sure you want to purchase: " << ss.str().c_str();
            points << " Points (" << sAlphaRewards->points[i] << ")";

            itempoints << ss.str().c_str() << points.str().c_str();

            uint32 action = i + 1;
            player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, itempoints.str().c_str(), page, action, confirm.str().c_str(), sAlphaRewards->points[i], false);
        }
        if (start > 0)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Previous page", page - 1, 0);
        if (end < sAlphaRewards->id.size())
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Next page", page + 1, 0);

        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {

        if (!sWorld.GetModuleBoolConfig("Alpha.Rewards.Enable", true))
            return false;

        if (player->IsInCombat())
        {
            ChatHandler(player->GetSession()).PSendSysMessage("Please Leave combat before speaking to me!");
            return true;
        }
        ShowPage(player, creature, 0);
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 page, uint32 action)
    {
        if (action > 0)
        {
            uint32 i = action - 1;
            sAlphaRewards->AddItemAndCheck(player, sAlphaRewards->itemId[i], sAlphaRewards->quantity[i], sAlphaRewards->points[i]);
        }
        player->PlayerTalkClass->ClearMenus();
        ShowPage(player, creature, page);
        return true;
    }

    struct alpha_reward_system_npcAI : public ScriptedAI
    {
        alpha_reward_system_npcAI(Creature* c) : ScriptedAI(c) {}

        uint32 say_timer;
        bool canSay;

        void Reset()
        {
            say_timer = 1000;
            canSay = false;
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (me->IsWithinDist(who, 5.0f) && who->GetTypeId() == TYPEID_PLAYER)
            {
                canSay = true;
            }
            else
                canSay = false;
        }

        void UpdateAI(uint32 diff)
        {
            if (say_timer <= diff)
            {
                if (canSay)
                {
                    me->MonsterSay("Hey stranger! do you have any gamepoints, speak with me to make a deal!", LANG_UNIVERSAL, NULL);
                    me->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                    say_timer = 61000;
                }
            }
            else
                say_timer -= diff;
        }

    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new alpha_reward_system_npcAI(creature);
    }
};

void AddSC_alpha_reward_system_npc()
{
    new alpha_reward_system_npc();
}
