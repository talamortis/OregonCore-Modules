#ifndef ALPHAREWARDS_H
#define ALPHAREWARDS_H

#include "ScriptMgr.h"
#include "Language.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "Group.h"
#include "Map.h"
#include "Log.h"
#include "Opcodes.h"
#include <vector>
#include <algorithm>
#include "BattlegroundMgr.h"
#include "AccountMgr.h"
#include <map>
#include <unordered_map>
#include "Configuration/Config.h"

class AlphaRewards
{
public:
    void OnLoginCheck(Player* player);
    void OnLogoutUpdate(Player* player);
    void AddGamePoint(Player* player, uint32 game_point);
    void LoadAlphaRewardsTable();
    void LoadAlphaRewardItemTable();
    void AddItemAndCheck(Player* player, uint32 item, uint32 quantity, uint32 points);
    void UpdateDBGamePoint(ChatHandler* handler, char const* account, uint32 points);
    std::unordered_map<uint32, uint32> AlphaQuestPointsMap;
    std::unordered_map<uint32, uint32> AlphaCreaturePointsMap;
    std::unordered_map<uint32, uint32> AlphaAchievementPointsMap;

    std::vector<uint32>id;
    std::vector<uint32>itemId;
    std::vector<uint32>points;
    std::vector<uint32>quantity;
};

class AlphaRewardData : public DataMap::Base
{
public:
    AlphaRewardData() {}
    AlphaRewardData(uint32 RewardPointsMap) : RewardPointsMap(RewardPointsMap) { reset(); }
    uint32 RewardPointsMap = 0;
    uint32 timer = sWorld.GetModuleIntConfig("RewardPlayedTime", 2)*HOUR*IN_MILLISECONDS; // make this config Driven and rmoev below void Reset();

    void reset()
    {
        timer = static_cast <uint32>(sWorld.GetModuleIntConfig("Alpha.Reward.PlayedTime", 2))* HOUR*IN_MILLISECONDS;
    }
};

#define sAlphaRewards ACE_Singleton<AlphaRewards, ACE_Null_Mutex>::instance()

#endif


/*
Bit of documentation,

- World Database table alpha reward system runs from a type on how to earn rewards

1. is creature kill
2. is quest
3. is achivement

World Database Alpha Reward Items is for the npc shop
- id = Max will be 14 per menu
- ItemId = item Id of the item
- Points = How much will the points cost to the player
- Quantity - How many items the player will recieve

*/
