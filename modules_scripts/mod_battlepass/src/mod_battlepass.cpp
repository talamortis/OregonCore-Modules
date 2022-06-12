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
    BattlePassPlayerInfo() {}
    uint32 Quest_Complted;
    uint32 pvpKills;
    uint32 battlepassLevel;
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

    auto it = mRewards.find(playerBattlePassInfo->battlepassLevel);

    if (it == mRewards.end())
        return;

    ItemTemplate const* itemProto = sItemStorage.LookupEntry<ItemTemplate>(it->second.reward);

    switch (it->second.option)
    {
    case 1: // Item
        sBattlePass->PlayerAddItem(player, it->second.reward, it->second.amount);
        ChatHandler(player).PSendSysMessage(" %s has been sent to your ingame mailbox", itemProto->Name1);
        break;
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
        return;
    }
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

class mod_BattlePass_Player : public PlayerScript
{
public:
    mod_BattlePass_Player() : PlayerScript("mod_BattlePass_Player") { }

    void OnLogin(Player* player, bool firstLogin) override
    {
        QueryResult_AutoPtr result = CharacterDatabase.PQuery("SELECT `playerGUID`, `BattlePassLevel`, `CompletedQuest`, `PvPKills` FROM `battlepass_player` WHERE `playerGUID` = '%u' ", player->GetGUID());

        if (!result)
            return;
        do
        {
            Field* fields = result->Fetch();
            sBattlePass->pGUID.push_back(fields[0].GetUInt32());

            BattlePassPlayerInfo* playerBattlePassInfo = player->CustomData.GetDefault<BattlePassPlayerInfo>("BattlePassPlayerInfo");
            playerBattlePassInfo->battlepassLevel = fields[1].GetUInt32();
            playerBattlePassInfo->Quest_Complted = fields[2].GetUInt32();
            playerBattlePassInfo->pvpKills = fields[3].GetUInt32();

            
        } while (result->NextRow());
    }

    void OnPlayerCompleteQuest(Player* player, Quest const* quest)
    {
        if (!player)
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

        BattlePassPlayerInfo* playerBattlePassInfo = player->CustomData.GetDefault<BattlePassPlayerInfo>("BattlePassPlayerInfo");

        // Appky new updates to DB on logout
        CharacterDatabase.PExecute("UPDATE `battlepass_player` SET `BattlePassLevel` = %u, `CompletedQuest` = %u, `PvPKills` = %u WHERE `playerGUID` = %u;",
            playerBattlePassInfo->battlepassLevel, playerBattlePassInfo->Quest_Complted, playerBattlePassInfo->pvpKills, player->GetGUID());
        sBattlePass->Erase(player);
    }
};

class LoadRewardsTable : public WorldScript
{
public:
    LoadRewardsTable() : WorldScript("load_rewards_table") { }

    void OnLoadCustomDatabaseTable()
    {
        uint32 oldMSTime = getMSTime();

        QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT `id`, `option`, `reward`, `amount` FROM `battlepass_rewards`");

        if (!result)
        {
            sLog.outString("BattlePass_Rewards Table is Empty");
            sLog.outString("");
            return;
        }

        uint32 count = 0;
        do
        {
            Field* fields = result->Fetch();
            mRewardsMap rmap;
            rmap.id = fields[0].GetUInt32();
            rmap.option = fields[1].GetUInt32();
            rmap.reward = fields[2].GetUInt32();
            rmap.amount = fields[3].GetUInt32();

            sBattlePass->mRewards[rmap.id] = rmap;

            ++count;
        } while (result->NextRow());

        sLog.outString("server.loading", ">> Loaded %u Battlepass Rewards in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        sLog.outString("server.loading", " ");
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

        if (pGUID)
            return false;

        // Now we have executed into the DB
        CharacterDatabase.PQuery("INSERT INTO `battlepass_player` (playerGUID) VALUES (%u);", pGUID);
        // We need to put in the Map
        sBattlePass->pGUID.push_back(pGUID);
        // Now we have activated this item we need to remove it
        player->DestroyItemCount(item->GetEntry(), 1, true);
        return true;
    }
};

void Addmod_battlepassScripts()
{
    new LoadRewardsTable();
    new mod_BattlePass_Player();
    new BattlePassItem();
}