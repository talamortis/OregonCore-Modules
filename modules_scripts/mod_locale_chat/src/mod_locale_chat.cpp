#include "WorldSession.h"
#include "Player.h"
#include "ChannelMgr.h"
#include "Channel.h"
#include "Chat.h"
#include "ScriptMgr.h"
#include "SocialMgr.h"
#include "Utilities/DataMap.h"

/* Class Colors */
std::string locale_chat_ClassColor[11] =
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

std::string locale_chat_TeamIcon[2] =
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

class chat_locale_data : public DataMap::Base
{
public:
    chat_locale_data() {}

    chat_locale_data(bool enabled) : enable(enabled) {}
    bool enable;
};

class chat_locale : public CommandScript
{
public:
    chat_locale() : CommandScript("chat_locale") { }

    static bool HandleLocaleCommand(ChatHandler* pChat, const char* msg)
    {
        Player* player = pChat->GetSession()->GetPlayer();

        if (!*msg)
            return false;

        chat_locale_data* Playerdata = player->CustomData.GetDefault<chat_locale_data>("chat_locale_data");

        if (!Playerdata->enable)
        {
            ChatHandler(player).PSendSysMessage("Localized chat is currently disabled. Please enable it");
            return true;
        }

        SessionMap sessions = sWorld.GetAllSessions();

        for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
        {
            if (!itr->second || !itr->second->GetPlayer() || !itr->second->GetPlayer()->IsInWorld())
                continue;

            Player* target = itr->second->GetPlayer();
            chat_locale_data* targetData = target->CustomData.GetDefault<chat_locale_data>("chat_locale_data");

            std::ostringstream ss;

            ss << channel_colour.c_str() << "[World] [" << locale[player->GetSession()->GetSessionDbcLocale()] << "] [|r" << locale_chat_TeamIcon[player->GetTeamId()] << "|r" << 
                channel_colour.c_str() << "] [" << locale_chat_ClassColor[player->getClass() - 1] << "|Hplayer:" << player->GetName() << "|h" <<  player->GetName() << "|h|r" << channel_colour << "]: " << msg; 

            if (target->IsGameMaster())
                ChatHandler(target->GetSession()).PSendSysMessage(ss.str().c_str());
            else
            {
                if (pChat->GetSession()->GetSessionDbcLocale() != target->GetSession()->GetSessionDbcLocale())
                    continue;

                // allow crossfaction chat.
                if (!sWorld.GetModuleBoolConfig("Chatlocale.AllowTwoSide", true))
                    if (player->GetTeam() != target->GetTeam())
                        continue;

                // Check see if target is ignoring sender.
                if (target->GetSocial()->HasIgnore(player->GetGUIDLow()))
                    continue;

                // Check see if target has enabled or disabled locale chat
                if (!targetData->enable)
                    continue;

                ChatHandler(target->GetSession()).PSendSysMessage(ss.str().c_str());
            }
        }
        return true;
    }

    static bool HandleLocaleChatOnCommand(ChatHandler* pChat, const char* msg)
    {
        Player* player = pChat->GetSession()->GetPlayer();

        chat_locale_data* data = player->CustomData.GetDefault<chat_locale_data>("chat_locale_data");

        data->enable = true;

        ChatHandler(player).PSendSysMessage("You have enabled localized chat");

        return true;
    }

    static bool HandleLocaleChatOffCommand(ChatHandler* pChat, const char* msg)
    {
        Player* player = pChat->GetSession()->GetPlayer();

        chat_locale_data* data = player->CustomData.GetDefault<chat_locale_data>("chat_locale_data");

        data->enable = false;

        ChatHandler(player).PSendSysMessage("You have disabled localized chat");

        return true;
    }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> localeChat_sub =
        {
            { "on",      SEC_PLAYER,     false,    &HandleLocaleChatOnCommand,      ""},
            { "off",     SEC_PLAYER,     false,    &HandleLocaleChatOffCommand,       ""},
            { "",        SEC_PLAYER,     false,    &HandleLocaleCommand,       ""}
        };

        static std::vector<ChatCommand> cmdtable =
        {
            { "locale", SEC_PLAYER, true, NULL , "", localeChat_sub},
        };

        return cmdtable;
    }
};

void Addmod_localeScripts()
{
    new chat_locale();
}