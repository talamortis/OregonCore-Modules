#include "ScriptMgr.h"
#include "Player.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "QuestDef.h"
#include "DataMap.h"
#include "mod-alpha-rewards.h"
#include "ace/ACE.h"
#include <map>
#include <string>
#include "Configuration/Config.h"
#include <list>

void AlphaRewards::OnLoginCheck(Player* player)
{
    if (!sWorld.GetModuleBoolConfig("Alpha.Rewards.Enable", true))
        return;

    QueryResult* result = LoginDatabase.PQuery("SELECT `game_point` FROM `account` WHERE `id` = '%u' ", player->GetSession()->GetAccountId());

    if (!result)
        return;

    Field* fields = result->Fetch();

    player->CustomData.Set("RewardPointsMap", new AlphaRewardData(fields[0].GetUInt32()));
}

void AlphaRewards::OnLogoutUpdate(Player* player)
{
    if (!sWorld.GetModuleBoolConfig("Alpha.Rewards.Enable", true))
        return;

    if (AlphaRewardData* data = player->CustomData.Get<AlphaRewardData>("RewardPointsMap"))
    {
        uint32 reward = data->RewardPointsMap;
        LoginDatabase.DirectPExecute("UPDATE `account` SET `game_point` = '%u' WHERE `id` = '%u'", reward, player->GetSession()->GetAccountId());
    }
}

void AlphaRewards::AddGamePoint(Player* player, uint32 game_point)
{
    player->CustomData.GetDefault<AlphaRewardData>("RewardPointsMap")->RewardPointsMap += game_point;
}

void AlphaRewards::LoadAlphaRewardsTable()
{
    if (!sWorld.GetModuleBoolConfig("Alpha.Rewards.Enable", true))
        return;

    sLog.outString("Loading Alpha Reward System...");

    QueryResult* result = WorldDatabase.PQuery("SELECT `Entry`, `TypeId`, `Points` FROM `alpha_reward_system`");

    if (!result)
    {
        sLog.outErrorDb(">>  Loaded 0 Alpha Rewards. DB table `alpha_reward_system` is empty!");
        sLog.outString();
        return;
    }

    uint32 count = 0;
    uint32 oldMSTime = getMSTime();

    do
    {
        Field* field = result->Fetch();

        switch (field[1].GetInt32())
        {
        case 1: // Creature
            sAlphaRewards->AlphaCreaturePointsMap.emplace(field[0].GetUInt32(), field[2].GetUInt32());
            break;
        case 2: // Quest Map
            sAlphaRewards->AlphaQuestPointsMap.emplace(field[0].GetUInt32(), field[2].GetUInt32());
            break;
        }

        count++;

    } while (result->NextRow());

    sLog.outString(">> Loaded %u Alpha Rewards in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    sLog.outString("");
}

void AlphaRewards::LoadAlphaRewardItemTable()
{
    if (!sWorld.GetModuleBoolConfig("Alpha.Rewards.Enable", true))
        return;

    sLog.outString("Loading Alpha Rewards Item Table...");

    QueryResult* result = WorldDatabase.PQuery("SELECT `id`, `itemId`, `points`, `quantity` FROM `alpha_reward_system_items`");

    if (!result)
    {
        sLog.outErrorDb(">>  Loaded 0 Alpha Rewards Items. DB table `alpha_reward_system_items` is empty!");
        sLog.outString();
        return;
    }

    uint32 count = 0;
    uint32 oldMsTime = getMSTime();

    do
    {
        Field* fields = result->Fetch();

        id.push_back(fields[0].GetUInt32());
        itemId.push_back(fields[1].GetUInt32());
        points.push_back(fields[2].GetUInt32());
        quantity.push_back(fields[3].GetUInt32()); 

        count++;

    } while (result->NextRow());

    sLog.outString(">> Loaded %u Alpha Reward items in %u ms", count, GetMSTimeDiffToNow(oldMsTime));
    sLog.outString("");
}

void AlphaRewards::AddItemAndCheck(Player* player, uint32 item, uint32 quantity, uint32 points)
{
    if (!sWorld.GetModuleBoolConfig("Alpha.Rewards.Enable", true))
        return;

     uint32& _points = player->CustomData.GetDefault<AlphaRewardData>("RewardPointsMap")->RewardPointsMap;

     if (_points < points)
     {
         ChatHandler(player->GetSession()).PSendSysMessage("You do not have enough points");
         return;
     }

     // Lets check bag spot see if they have bag space
     uint32 noSpaceForCount = 0;
     ItemPosCountVec dest;
     uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, item, quantity, &noSpaceForCount);

     if (msg != EQUIP_ERR_OK)                               // convert to possible store amount
         quantity -= noSpaceForCount;

     if (quantity == 0 || dest.empty())                         // can't add any
     {
         ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_CANNOT_CREATE, item, noSpaceForCount);
         ChatHandler(player->GetSession()).SetSentErrorMessage(true);
         return;
     }

     if (noSpaceForCount > 0)
         ChatHandler(player->GetSession()).PSendSysMessage(LANG_ITEM_CANNOT_CREATE, item, noSpaceForCount);

     if (quantity > 0 && item)
     {
         player->AddItem(item, quantity);

         _points -= points;

         sObjectMgr.GetItemTemplate(item)->Name1;

         // Tell the player his new Game points after buying items
         ChatHandler(player->GetSession()).PSendSysMessage("You brought '%s' for %u points, you have %u gamepoints remaining.", sObjectMgr.GetItemTemplate(item)->Name1, points, _points);
     }
}

void AlphaRewards::UpdateDBGamePoint(ChatHandler* handler, char const* account, uint32 point)
{
    QueryResult* result = LoginDatabase.PQuery("SELECT `id`, `username` FROM `account` WHERE `username` = '%s'", account);

    if (!result)
    {
        handler->PSendSysMessage("The account name '%s' does not exist", account);
        return;
    }

    Field* fields = result->Fetch();
    uint32 AccountId = fields[0].GetInt32();

    LoginDatabase.DirectPExecute("UPDATE `account` SET `game_point` =  `game_point` + '%u' WHERE `id` = '%u'", point, AccountId);
    handler->PSendSysMessage("You have given account %s %u Gamepoints", account, point);
}
