#include "ScriptMgr.h"
#include "Player.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "QuestDef.h"
#include "DataMap.h"
#include "mod-alpha-rewards.h"
#include "ace/ACE.h"
#include "Group.h"

class LoadAlphaRewardGlobal : public WorldScript
{
public:
    LoadAlphaRewardGlobal() : WorldScript("LoadAlphaRewardGlobal") { }

    void OnLoadCustomDatabaseTable() override
    {
        sAlphaRewards->LoadAlphaRewardsTable();
        sAlphaRewards->LoadAlphaRewardItemTable();
    }
};

class AlphaRewardPlayer : public PlayerScript
{
public:
    AlphaRewardPlayer() : PlayerScript("AlphaRewardPlayer") { }

    //On Login Get Current GamePoints
    void OnLogin(Player* p, bool /*First Login*/) override
    {
        sAlphaRewards->OnLoginCheck(p);
    }

    //On logout update the gamepoints.
    void OnLogout(Player* p) override
    {
        sAlphaRewards->OnLogoutUpdate(p);
    }

    // Reward for Active Game Time staying logged in
    void OnBeforeUpdate(Player* player, uint32 p_time) override
    {
        if (player->isAFK())
            return;

        AlphaRewardData* RewardPoint = player->CustomData.GetDefault<AlphaRewardData>("RewardPoint");

        if (RewardPoint->timer <= p_time)
        {
            RewardPoint->reset();
            sAlphaRewards->AddGamePoint(player, 1);
        }
            
    }

    void OnPlayerCompleteQuest(Player* player, Quest const* quest) override
    {
        auto it = sAlphaRewards->AlphaQuestPointsMap.find(quest->GetQuestId());

        if (it != sAlphaRewards->AlphaQuestPointsMap.end())
            sAlphaRewards->AddGamePoint(player, it->second);
    }

    void OnCreatureKill(Player* killer, Creature* killed) override
    {
        Group* grp = killer->GetGroup();
        if (grp)
        {
            for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* member = itr->GetSource();
                if (!member || !member->GetSession() || !member->IsWithinDist(killer, 25.0f))
                    continue;

                auto it = sAlphaRewards->AlphaCreaturePointsMap.find(killed->GetEntry());

                if (it != sAlphaRewards->AlphaCreaturePointsMap.end())
                    sAlphaRewards->AddGamePoint(member, it->second);
            }
        }
        else
        {
            auto it = sAlphaRewards->AlphaCreaturePointsMap.find(killed->GetEntry());

            if (it != sAlphaRewards->AlphaCreaturePointsMap.end())
                sAlphaRewards->AddGamePoint(killer, it->second);
        }
    }

    /*void OnAchiComplete(Player* player, AchievementEntry const* achievement) override
    {
        auto it = sAlphaRewards->AlphaAchievementPointsMap.find(achievement->ID);

        if (it != sAlphaRewards->AlphaAchievementPointsMap.end())
            sAlphaRewards->AddGamePoint(player, it->second);
    } */
};

void AddSC_AlphaRewardScript()
{
    new AlphaRewardPlayer();
    new LoadAlphaRewardGlobal();
    //new AlphaRewardGlobalScript();
}
