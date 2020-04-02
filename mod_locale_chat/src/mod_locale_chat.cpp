#include "WorldSession.h"
#include "Player.h"
#include "ChannelMgr.h"
#include "Channel.h"
#include "Chat.h"
#include "Config.h"
#include "ScriptMgr.h"

/* Class Colors */
std::string world_chat_ClassColor[11] =
{
    "|cffC79C6E", // WARRIOR
    "|cffF58CBA", // PALADIN
    "|cffABD473", // HUNTER
    "|cffFFF569", // ROGUE
    "|cffFFFFFF", // PRIEST
    "|cffC41F3B", // DEATHKNIGHT
    "|cff0070DE", // SHAMAN
    "|cff40C7EB", // MAGE
    "|cff8787ED", // WARLOCK
    "", // ADDED IN MOP FOR MONK - NOT USED
    "|cffFF7D0A", // DRUID
};

std::string world_chat_TeamIcon[2] =
{
    "|cff3399FFAlliance|r",
    "|cffCC0000Horde|r"
};

std::string locale[9] =
{
    "EN",
    "KO",
    "FR",
    "DE",
    "CN",
    "TW",
    "ES",
    "MX",
    "RU"
};

std::string channel_colour = "|cffFEC1C0";

class chat_locale : public CommandScript
{
public:
    chat_locale() : CommandScript("chat_locale") { }

    static bool HandleLocaleCommand(ChatHandler* pChat, const char* msg)
    {
        Player* player = pChat->GetSession()->GetPlayer();

        if (!*msg)
            return false;

        SessionMap sessions = sWorld.GetAllSessions();

        for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
        {
            if (!itr->second || !itr->second->GetPlayer() || !itr->second->GetPlayer()->IsInWorld())
                continue;

            Player* target = itr->second->GetPlayer();

            std::ostringstream ss;

            ss << channel_colour.c_str() << "[World] [" << locale[player->GetSession()->GetSessionDbcLocale()] << "] [|r" << world_chat_TeamIcon[player->GetTeamId()] << "|r" << 
                channel_colour.c_str() << "] [" << world_chat_ClassColor[player->getClass() - 1] << "|Hplayer:" << player->GetName() << "|h" <<  player->GetName() << "|h|r" << channel_colour << "]: " << msg; 

            if (target->IsGameMaster())
                ChatHandler(target->GetSession()).PSendSysMessage(ss.str().c_str());
            else
            {
                if (pChat->GetSession()->GetSessionDbcLocale() != target->GetSession()->GetSessionDbcLocale())
                    continue;

                if (!sWorld.GetModuleBoolConfig("Chatlocale.AllowTwoSide", true))
                    if (player->GetTeam() != target->GetTeam())
                        continue;

                ChatHandler(target->GetSession()).PSendSysMessage(ss.str().c_str());
            }
        }
        return true;
    }

    ChatCommand* GetCommands() const
    {
        static ChatCommand cmdtable[] =
        {
            { "locale", SEC_PLAYER, true, &HandleLocaleCommand , "", NULL},
        };

        return cmdtable;
    }
};

void Addmod_localeScripts()
{
    new chat_locale();
}