#ifndef ISPREMIUM_H
#define ISPREMIUM_H

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
#include <map>

struct mRewardsMap
{
    uint32 id;
    uint32 option;
    uint32 reward;
    uint32 amount;
};

class BattlePassInfo
{
public:
    void BattlePassLevelUp(Player* player);
    bool DoesHaveBattlePass(Player* player);
    void Erase(Player* player);
    void PlayerAddItem(Player* player, uint32 item, uint32 amount);
    std::vector<uint32>pGUID;
    UNORDERED_MAP<uint32, mRewardsMap> mRewards;

};

#define sBattlePass ACE_Singleton<BattlePassInfo, ACE_Null_Mutex>::instance()

#endif