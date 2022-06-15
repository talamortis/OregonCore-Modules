#include "ScriptMgr.h"
#include "Configuration/Config.h"
#include "Player.h"
#include "World.h"
#include "Chat.h"
#include "mod_battlepass.h"
#include "Battleground.h"
#include "Mail.h"


class BattlePassPlayerInfo : public DataMap::Base
{
public:
    BattlePassPlayerInfo() { reset(); }
    uint32 Quest_Complted;
    uint32 pvpKills;
    uint32 battlepassLevel;

    uint32 timer = sWorld.GetModuleIntConfig("BattlePassAutoUpdate.Remove", 3) * MINUTE * IN_MILLISECONDS;

    void reset()
    {
        timer = static_cast <uint32>(sWorld.GetModuleIntConfig("BattlePassAutoUpdate", 3)) * MINUTE * IN_MILLISECONDS;
    }

};

void BattlePassInfo::PlayerAddItem(Player* player, uint32 item, uint32 amount)
{
    ItemTemplate const* proto = sObjectMgr.GetItemTemplate(item);

    MailDraft draft("BattlePass Item", "Thank you for your Support!");
        if (Item* pitem = Item::CreateItem(item, amount, player))
        {
            pitem->SaveToDB();                               // save for prevent lost at next mail load, if send fail then item will deleted
            draft.AddItem(pitem);
        }

    draft.SendMailTo(MailReceiver(player), MailSender(MAIL_NORMAL, 0, MAIL_STATIONERY_GM), MAIL_CHECK_MASK_RETURNED);
}


void BattlePassInfo::BattlePassLevelUp(Player* player)
{
    BattlePassPlayerInfo* playerBattlePassInfo = player->CustomData.GetDefault<BattlePassPlayerInfo>("BattlePassPlayerInfo");
    playerBattlePassInfo->battlepassLevel++;

    ChatHandler(player).PSendSysMessage("|cff4CFF00Congratulations your new BattlePass Level is|cffFF0000 %u", playerBattlePassInfo->battlepassLevel);
    player->CastSpell(player, 24312, false);
    player->RemoveAurasDueToSpell(24312);

    for (auto it = mRewards.begin(); it != mRewards.end(); ++it)
    {
        if (playerBattlePassInfo->battlepassLevel != it->second.id)
            continue;

        switch (it->second.option)
        {
        case 1:
        {
            if (it->second.reward == 0)
                return;

            ItemTemplate const* itemProto = sItemStorage.LookupEntry<ItemTemplate>(it->second.reward);
            sBattlePass->PlayerAddItem(player, it->second.reward, it->second.amount);
            ChatHandler(player).PSendSysMessage(" %s x %u has been sent to your ingame mailbox", itemProto->Name1, it->second.amount);
            continue;
        }
        case 2: // Gold
            player->ModifyMoney(it->second.reward);
            ChatHandler(player).PSendSysMessage("%u has been added to your character", it->second.reward);
            break;
        case 3: //levels
            if (player->getLevel() == sWorld.getConfig(CONFIG_MAX_PLAYER_LEVEL))
            {
                ChatHandler(player).PSendSysMessage("You are already Max level, Unable to give this tier reward");
                return;
            }
            player->SetLevel(player->getLevel() + it->second.reward);
            ChatHandler(player).PSendSysMessage("You have been awarded %1 for tiering up!", it->second.reward);
            break;
        default:
            break;
        }

    }

    // Now We have update, Lets update the player Level in the battle Pass to save progression
    UpdateBattlePassTable(player);
}

void BattlePassInfo::UpdateBattlePassTable(Player* player)
{
    BattlePassPlayerInfo* playerBattlePassInfo = player->CustomData.GetDefault<BattlePassPlayerInfo>("BattlePassPlayerInfo");

    // update player Details
    CharacterDatabase.PExecute("UPDATE `battlepass_player` SET `BattlePassLevel` = %u, `CompletedQuest` = %u, `PvPKills` = %u WHERE `playerGUID` = %u;",
        playerBattlePassInfo->battlepassLevel, playerBattlePassInfo->Quest_Complted, playerBattlePassInfo->pvpKills, player->GetGUID());
}

bool BattlePassInfo::DoesHaveBattlePass(Player* player)
{
    std::vector<uint32>::iterator it;
    it = std::find(pGUID.begin(), pGUID.end(), player->GetGUID());

    if (it == pGUID.end())
        return false;

    return true;
}

void BattlePassInfo::Erase(Player* player)
{
    std::vector<uint32>::iterator it;
    it = std::find(pGUID.begin(), pGUID.end(), player->GetGUID());

    if (it != pGUID.end())
        pGUID.erase(it);
}

void BattlePassInfo::LoadCustomBattlePassTable()
{
    // Make sure table is empty due to multimap;
    mRewards.clear();

    uint32 oldMSTime = getMSTime();

    QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT `id`, `option`, `reward`, `amount` FROM `battlepass_rewards`");

    if (!result)
    {
        sLog.outString("BattlePass_Rewards Table is Empty");
        sLog.outString("");
        return;
    }

    count = 0;
    do
    {
        Field* fields = result->Fetch();
        mRewardsMap rmap;
        rmap.id = fields[0].GetUInt32();
        rmap.option = fields[1].GetUInt32();
        rmap.reward = fields[2].GetUInt32();
        rmap.amount = fields[3].GetUInt32();

        sBattlePass->mRewards.insert(std::make_pair(rmap.id, rmap));

        ++count;
    } while (result->NextRow());

    sLog.outString("server.loading", ">> Loaded %u Battlepass Rewards in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    sLog.outString("server.loading", " ");
}

class mod_BattlePass_Player : public PlayerScript
{
public:
    mod_BattlePass_Player() : PlayerScript("mod_BattlePass_Player") { }

    void OnBeforeUpdate(Player* player, uint32 p_time) override
    {
        BattlePassPlayerInfo* playerBattlePassInfo = player->CustomData.GetDefault<BattlePassPlayerInfo>("BattlePassPlayerInfo");

        playerBattlePassInfo->timer-= p_time;

        if (playerBattlePassInfo->timer <= p_time)
        {
            sBattlePass->UpdateBattlePassTable(player);
            if (sWorld.GetModuleBoolConfig("BattlePassAnnounceToPlayer", false))
                ChatHandler(player).PSendSysMessage("[AUTOSAVE] Battlepass progress has been saved.");

            playerBattlePassInfo->reset();
        }
    }

    void OnLogin(Player* player, bool firstLogin) override
    {
        QueryResult_AutoPtr result = CharacterDatabase.PQuery("SELECT `playerGUID`, `BattlePassLevel`, `CompletedQuest`, `PvPKills` FROM `battlepass_player` WHERE `playerGUID` = '%u' ", player->GetGUID());
        BattlePassPlayerInfo* playerBattlePassInfo = player->CustomData.GetDefault<BattlePassPlayerInfo>("BattlePassPlayerInfo");

        if (!result)
            return;
        do
        {
            Field* fields = result->Fetch();
            sBattlePass->pGUID.push_back(fields[0].GetUInt32());

            playerBattlePassInfo->battlepassLevel = fields[1].GetUInt32();
            playerBattlePassInfo->Quest_Complted = fields[2].GetUInt32();
            playerBattlePassInfo->pvpKills = fields[3].GetUInt32();

            
        } while (result->NextRow());

        playerBattlePassInfo->timer = sWorld.GetModuleIntConfig("BattlePassAutoUpdate.Remove", 3) * MINUTE * IN_MILLISECONDS;

    }

    void OnPlayerCompleteQuest(Player* player, Quest const* quest)
    {
        if (!player)
            return;

        // Do not reward for repeatable quests
        if (quest->IsRepeatable() && !quest->IsDaily())
            return;

        if (!sBattlePass->DoesHaveBattlePass(player))
            return;

        BattlePassPlayerInfo* playerBattlePassInfo = player->CustomData.GetDefault<BattlePassPlayerInfo>("BattlePassPlayerInfo");

        playerBattlePassInfo->Quest_Complted++;

        if (playerBattlePassInfo->Quest_Complted == sWorld.GetModuleIntConfig("BattlePassLevelQuest", 10))
        {
            sBattlePass->BattlePassLevelUp(player);
            playerBattlePassInfo->Quest_Complted = 0;
        }
    }

    void OnPVPKill(Player* killer, Player* killed)
    {
        if (!sBattlePass->DoesHaveBattlePass(killer))
            return;

        // If player kills self do not drop loot
        if (killer->GetGUID() == killed->GetGUID())
            return;

        if (killer->GetSession()->GetRemoteAddress() == killed->GetSession()->GetRemoteAddress())
            return;

        BattlePassPlayerInfo* playerBattlePassInfo = killer->CustomData.GetDefault<BattlePassPlayerInfo>("BattlePassPlayerInfo");
        playerBattlePassInfo->pvpKills++;

        if (playerBattlePassInfo->pvpKills == sWorld.GetModuleIntConfig("BattlePassLevelPvP", 30))
        {
            sBattlePass->BattlePassLevelUp(killer);
            playerBattlePassInfo->pvpKills = 0;
        }

    }

    void OnLogout(Player* player)
    {

        if (!sBattlePass->DoesHaveBattlePass(player))
            return;

        // Update the BattlePass Table on Logout
        sBattlePass->UpdateBattlePassTable(player);

        sBattlePass->Erase(player);
    }
};

class LoadRewardsTable : public WorldScript
{
public:
    LoadRewardsTable() : WorldScript("load_rewards_table") { }

    void OnLoadCustomDatabaseTable()
    {
        sBattlePass->LoadCustomBattlePassTable();
    }
};

class BattlePassItem : public ItemScript
{
public:
    BattlePassItem() : ItemScript("BattlePassItem") {}

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override
    {
        if (sBattlePass->DoesHaveBattlePass(player))
        {
            ChatHandler(player).PSendSysMessage("This Character already has battlepass");
            return false;
        }
        uint32 pGUID = player->GetGUID();

        if (!pGUID)
            return false;

        // Now we have executed into the DB
        CharacterDatabase.PQuery("INSERT INTO `battlepass_player` (playerGUID) VALUES (%u);", pGUID);
        ChatHandler(player).PSendSysMessage("You now are enrolled onto the battlepass");
        // We need to put in the Map
        sBattlePass->pGUID.push_back(pGUID);
        // Now we have activated this item we need to remove it
        player->DestroyItemCount(item->GetEntry(), 1, true);

        BattlePassPlayerInfo* playerBattlePassInfo = player->CustomData.GetDefault<BattlePassPlayerInfo>("BattlePassPlayerInfo");
        playerBattlePassInfo->battlepassLevel = 0;
        playerBattlePassInfo->pvpKills = 0;
        playerBattlePassInfo->Quest_Complted = 0;
        return true;
    }
};

class BattlePassCommand : public CommandScript
{
public:
    BattlePassCommand() : CommandScript("BattlePassCommandScript") {}

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> BattlePassCommandTable =
        {
            { "Save", SEC_PLAYER, true, &HandlePlayerSaveBattlePassProgress, "Save the Current Progress of the player battle Pass" },
            { "Level", SEC_ADMINISTRATOR, false, &HandleGrantBattlePassLevel,    "Grant the Selected Target  a battlepass level"},
            { "Reload", SEC_ADMINISTRATOR, false, &HandleBattlePassReload, "Reaload all items in the DB"}
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "BattlePass", SEC_PLAYER, false, nullptr, "", BattlePassCommandTable }
        };

        return commandTable;
    }

    static bool HandleBattlePassReload(ChatHandler* handler, char const* args)
    {
        sBattlePass->LoadCustomBattlePassTable();
        handler->PSendSysMessage("BattlePass_Rewards table relaoded with %u entry", sBattlePass->count);
        return true;
    }

    static bool HandleGrantBattlePassLevel(ChatHandler* handler, char const* args)
    {
        if (!handler)
            return false;

        Player* target = handler->getSelectedPlayer();
        if (!target)
            return false;
        
        if (!sBattlePass->DoesHaveBattlePass(target))
            return false;

        BattlePassPlayerInfo* playerBattlePassInfo = target->CustomData.GetDefault<BattlePassPlayerInfo>("BattlePassPlayerInfo");
        playerBattlePassInfo->pvpKills = 0;
        playerBattlePassInfo->Quest_Complted = 0;
        sBattlePass->BattlePassLevelUp(target);
        return true;

    }

    static bool HandlePlayerSaveBattlePassProgress(ChatHandler* handler, char const* args)
    {
        if (!handler)
            return false;

        Player* player = handler->GetSession()->GetPlayer();

        if (!player)
            return false;

        if (!sBattlePass->DoesHaveBattlePass(player))
            return false;

        sBattlePass->UpdateBattlePassTable(player);
        handler->PSendSysMessage("Your battlepass progress has been saved.");
        return true;
    }
};

void Addmod_battlepassScripts()
{
    new LoadRewardsTable();
    new mod_BattlePass_Player();
    new BattlePassItem();
    new BattlePassCommand();
}