//by SymbolixDEV
//Reworked by Talamortis
#include "ScriptMgr.h"
#include <Player.h>
#include "Guild.h"
#include "Group.h"

static bool removeAura, BossAnnouncerEnable, BossAnnounceToPlayerOnLogin;

class Boss_Announcer : public PlayerScript
{
public:
	Boss_Announcer() : PlayerScript("Boss_Announcer") {}

	bool BossAnnouncerEnable = sWorld.GetModuleBoolConfig("Boss.Announcer.Enable", true);
	bool removeAura = sWorld.GetModuleBoolConfig("Boss.Announcer.RemoveAuraUponKill", false);
	bool BossAnnounceToPlayerOnLogin = sWorld.GetModuleBoolConfig("Boss.Announcer.Announce", true);
	
    void OnLogin(Player* player, bool firstLogin)
    {
        if (BossAnnouncerEnable)
        {
            if (BossAnnounceToPlayerOnLogin)
            {
                ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00BossAnnouncer |rmodule.");
            }
        }
    }

    void OnCreatureKill(Player* player, Creature* boss)
    {
        if (BossAnnouncerEnable)
        {
			CreatureInfo const* BossInfo = sObjectMgr.GetCreatureTemplate(boss->GetEntry());

            if (boss->GetMap()->IsRaid() && boss->getLevel() > 70 && BossInfo->type_flags & CREATURE_TYPE_FLAG_BOSS_MOB)
            {
                //lets get the info we want
                Map* map = player->GetMap();
                std::string p_name;
                std::string g_name;
                std::string boss_name = boss->GetName();
                std::string IsHeroicMode;
                std::string tag_colour = "7bbef7";
                std::string plr_colour = "7bbef7";
                std::string guild_colour = "00ff00";
                std::string boss_colour = "ff0000";
                std::string alive_text = "00ff00";
                uint32 Alive_players = 0;
				uint32 Max_Allowed_Players = 0;
                Map::PlayerList const & playerlist = map->GetPlayers();

                if (!player->GetGroup())
                    p_name = player->GetName();
                else
                    p_name = player->GetGroup()->GetLeaderName();

				InstanceTemplate const* pInsance = sObjectMgr.GetInstanceTemplate(player->GetMapId());

				Max_Allowed_Players = pInsance->maxPlayers;

                if (player->GetMap()->IsHeroic())
                    IsHeroicMode = "|cffff0000Heroic|r";
                else
                    IsHeroicMode = "|cff00ff00Normal|r";

                std::ostringstream stream;

                for (Map::PlayerList::const_iterator itr = playerlist.begin(); itr != playerlist.end(); ++itr)
                {
                    if (!itr->GetSource())
                        continue;

                    if (itr->GetSource()->IsAlive())
                        Alive_players++;

                    if (removeAura == true)
                    {
                        uint32 buff = itr->GetSource()->GetTeamId() == TEAM_ALLIANCE ? 57723 : 57724;

						if (itr->GetSource()->HasAura(buff))
							itr->GetSource()->RemoveAurasDueToSpell(buff);
                    }

					Guild* pGuild = sObjectMgr.GetGuildById(itr->GetSource()->GetGuildId());

                    if (!player->GetGuildId())
                    {
                        // if we are in group lets get guild of the leader
                        if (player->GetGroup())
                        {
                            if (itr->GetSource()->GetGroup()->IsLeader(itr->GetSource()->GetGUID()))
                                if (!pGuild)
                                    g_name = "< No Guild >";
                                else
									
                                    g_name = pGuild->GetName();
                        }

                        g_name = "< No Guild >";
                    }
                    else
						g_name = pGuild->GetName();
                }

				stream << "|CFF" << tag_colour << "|r|cff" << plr_colour << " " << p_name << "|r's Guild |cff" << guild_colour << "" << g_name << "|r has slain |CFF" << boss_colour << "[" << boss_name << "]|r with remaining |cff" << alive_text << "" << Alive_players << " /" << Max_Allowed_Players << "|r players alive on " << IsHeroicMode << " mode |r";
                sWorld.SendServerMessage(SERVER_MSG_STRING, stream.str().c_str());


            }
        }
    }
};

void AddBoss_AnnouncerScripts()
{
	new Boss_Announcer;
}
