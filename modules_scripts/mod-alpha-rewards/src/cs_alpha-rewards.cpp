#include "mod-alpha-rewards.h"
#include "Player.h"
#include "Chat.h"
#include "DataMap.h"
#include "ObjectAccessor.h"
#include <string>
#include "AccountMgr.h"

class cs_AlphaReward : public CommandScript
{
public:
    cs_AlphaReward() : CommandScript("cs_AlphaRewards") {}

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> cs_AlphaRewardSubTable =
        {
            { "view", SEC_PLAYER, false, &handleViewCommand, ""},
            { "add", SEC_ADMINISTRATOR, true, &handleAddCommand, ""},
        };

        static std::vector<ChatCommand> cs_AlphaRewardMainTable =
        {
            { "gamepoint", SEC_PLAYER, false, nullptr, "", cs_AlphaRewardSubTable}
        };

        return cs_AlphaRewardMainTable;
    }

    static bool handleViewCommand(ChatHandler* handler, char const* args)
    {
        if (!sWorld.GetModuleBoolConfig("Alpha.Rewards.Enable", true))
        {
            handler->PSendSysMessage("Reward Module is currently disabled.");
            return false;
        }


        Player* player = handler->GetSession()->GetPlayer();
        Player* target = player->GetSelectedPlayer();
        bool IsGM = player->GetSession()->GetSecurity() >= SEC_GAMEMASTER;

        if (target && IsGM) // lets get selected player
        {
            handler->PSendSysMessage("Player: %s Currently has %u GamePoints", target->GetName(), target->CustomData.GetDefault< AlphaRewardData>("RewardPointsMap")->RewardPointsMap);
        }
        else if (*args && IsGM && !target) // if no target selected but if arguments
        {
            //Check see if player is online
            Player* CheckPlayer = ObjectAccessor::Instance().FindPlayerByName((char*)args);

            if (CheckPlayer)
            {
                handler->PSendSysMessage("Player: %s currently has %u Gamepoints", CheckPlayer->GetName(), CheckPlayer->CustomData.GetDefault<AlphaRewardData>("RewardPointsMap")->RewardPointsMap);
            }
            else
            {
                std::string str = args;
                str[0] = toupper(str[0]);
                QueryResult* GetAccountId = CharacterDatabase.PQuery("SELECT `account` FROM `characters` WHERE `name` = '%s'", str.c_str());

                if (GetAccountId)
                {
                    Field* field2 = GetAccountId->Fetch();
                    uint32 AccountID = field2[0].GetInt32();

                    QueryResult* result = LoginDatabase.PQuery("SELECT `game_point` FROM `account` WHERE `id` = '%u'", AccountID);
                    Field* fields = result->Fetch();

                    if (result)
                    {
                        handler->PSendSysMessage("Player %s currently has %u gamepoints", str.c_str(), fields[0].GetUInt32());

                    }
                }
                else return false;
            }
        }
        else // No target or arguments get own
            handler->PSendSysMessage("You have currently got %u gamepoints", player->CustomData.GetDefault<AlphaRewardData>("RewardPointsMap")->RewardPointsMap);

        return true;
    }

    static bool handleAddCommand(ChatHandler* handler, char const* args)
    {
        if (!sWorld.GetModuleBoolConfig("Alpha.Rewards.Enable", true))
        {
            handler->PSendSysMessage("Reward Module is currently disabled.");
            return false;
        }

        Player* target = handler->getSelectedPlayer();

        char* accountName = strtok((char*)args, " ");
        char* amountstr = strtok(nullptr, " ");

        if (!accountName || !amountstr)
            return false;

        uint32 amount = atoi(amountstr);

        auto* session = sWorld.FindSession(sAccountMgr->GetId(accountName));

        if (session)
        {
            Player* player = session->GetPlayer();

            if (player && player->IsInWorld())
            {
                uint32& _points = player->CustomData.GetDefault<AlphaRewardData>("RewardPointsMap")->RewardPointsMap;
                _points += amount;
                handler->PSendSysMessage("You have given account %s %u Gamepoints", accountName, amount);
            }
            else
                // If player is in session but not logged into world update the DB table
                sAlphaRewards->UpdateDBGamePoint(handler, accountName, amount);
        }
        else
            sAlphaRewards->UpdateDBGamePoint(handler, accountName, amount);
        return true;
    }
};


void AddSC_AlphaCommand()
{
    new cs_AlphaReward();
}
