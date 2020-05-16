/*
<--------------------------------------------------------------------------->
- Developer(s): WiiZZy
- Complete: 100%
- ScriptName: 'World chat'
- Comment: Fully tested
<--------------------------------------------------------------------------->
*/

#include "ScriptMgr.h"
#include "Log.h"
#include "Player.h"
#include "Chat.h"
#include "Common.h"
#include "World.h"
#include "WorldSession.h"
#include <unordered_map>

/* VERSION */
float ver = 1.1f;

/* Colors */
std::string WORLD_CHAT_ALLIANCE_BLUE = "|cff3399FF";
std::string WORLD_CHAT_HORDE_RED = "|cffCC0000";
std::string WORLD_CHAT_WHITE = "|cffFFFFFF";
std::string WORLD_CHAT_GREEN = "|cff00CC00";
std::string WORLD_CHAT_RED = "|cffFF0000";
std::string WORLD_CHAT_BLUE = "|cff6666FF";
std::string WORLD_CHAT_BLACK = "|cff000000";
std::string WORLD_CHAT_GREY = "|cff808080";

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

/* Ranks */
std::string world_chat_GM_RANKS[4] =
{
    "Player",
    "MOD",
    "GM",
    "ADMIN",
};

/* BLIZZARD CHAT ICON FOR GM'S */
std::string world_chat_GMIcon = "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:15:15:0:-1|t";

/* COLORED TEXT FOR CURRENT FACTION || NOT FOR GMS */
std::string world_chat_TeamIcon[2] =
{
    "|cff3399FFAlliance|r",
    "|cffCC0000Horde|r"
};


/* STRUCTURE FOR WorldChat map */
struct ChatElements
{
    uint8 chat = (sWorld.GetModuleBoolConfig("World_Chat.OnLogin.State", true)) ? 1 : 0; // CHAT DISABLED BY DEFAULT
};

/* UNORDERED MAP FOR STORING IF CHAT IS ENABLED OR DISABLED */
std::unordered_map<uint32, ChatElements>WorldChat;


class WorldChat_commandscript : public CommandScript
{
public:
    WorldChat_commandscript() : CommandScript("example_commandscript") { }

    static bool HandleWorldChatCommand(ChatHandler * pChat, const char * msg)
    {
        Player* _player = pChat->GetSession()->GetPlayer();

        if (!sWorld.GetModuleBoolConfig("World_Chat.Enable", true)) {
            ChatHandler(_player).PSendSysMessage("[WC] %sWorld Chat System is disabled.|r", WORLD_CHAT_RED.c_str());
            return true;
        }

        if (!*msg)
            return false;

        uint64 guid = _player->GetGUID();

		if (!_player->CanSpeak()){
			ChatHandler(_player).PSendSysMessage("[WC] %sYou can't use World Chat while muted!|r", WORLD_CHAT_RED.c_str());
			return true;
		}

        if (!WorldChat[guid].chat) {
            ChatHandler(_player).PSendSysMessage("[WC] %sWorld Chat is disabled. (.chat)|r", WORLD_CHAT_RED.c_str());
            return true;
        }

        char message[1024];


        SessionMap sessions = sWorld.GetAllSessions();

        for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
        {
            if (!itr->second)
                continue;
            if (!itr->second->GetPlayer())
            {
                continue;
            }
            if(!itr->second->GetPlayer()->IsInWorld())
            {
                continue;
            }


            Player* target = itr->second->GetPlayer();
            uint64 guid2 = target->GetGUID();

            if (WorldChat[guid2].chat == 1)
            {
                if (sWorld.GetModuleBoolConfig("World_Chat.CrossFactions", true)) {
                    if (_player->IsGameMaster()) {
                        snprintf(message, 1024, "[World][%s][%s%s|r]: %s%s|r", world_chat_GMIcon.c_str(), world_chat_ClassColor[_player->getClass() - 1].c_str(), _player->GetName(), WORLD_CHAT_WHITE.c_str(), msg);
                    }
                    else
                        snprintf(message, 1024, "[World][%s][%s%s|r]: %s%s|r", world_chat_TeamIcon[_player->GetTeamId()].c_str(), world_chat_ClassColor[_player->getClass() - 1].c_str(), _player->GetName(), WORLD_CHAT_WHITE.c_str(), msg);
                    ChatHandler(target->GetSession()).PSendSysMessage("%s", message);
                }
                else
                {
                    if (_player->GetTeamId() == target->GetTeamId())
                    {
                        if (_player->IsGameMaster()) {
                            snprintf(message, 1024, "[World][%s][%s%s|r]: %s%s|r", world_chat_GMIcon.c_str(), world_chat_ClassColor[_player->getClass() - 1].c_str(), _player->GetName(), WORLD_CHAT_WHITE.c_str(), msg);
                        }
                        else
                            snprintf(message, 1024, "[World][%s][%s%s|r]: %s%s|r", world_chat_TeamIcon[_player->GetTeamId()].c_str(), world_chat_ClassColor[_player->getClass() - 1].c_str(), _player->GetName(), WORLD_CHAT_WHITE.c_str(), msg);
                        ChatHandler(target->GetSession()).PSendSysMessage("%s", message);
                    }
                    else if (target->IsGameMaster()) {
                        if (_player->IsGameMaster()) {
                            snprintf(message, 1024, "[World][%s][%s%s|r]: %s%s|r", world_chat_GMIcon.c_str(), world_chat_ClassColor[_player->getClass() - 1].c_str(), _player->GetName(), WORLD_CHAT_WHITE.c_str(), msg);
                        }
                        else
                            snprintf(message, 1024, "[World][%s][%s%s|r]: %s%s|r", world_chat_TeamIcon[_player->GetTeamId()].c_str(), world_chat_ClassColor[_player->getClass() - 1].c_str(), _player->GetName(), WORLD_CHAT_WHITE.c_str(), msg);
                        ChatHandler(target->GetSession()).PSendSysMessage("%s", message);
                    }
                }
            }
        }

        return true;
    }

    static bool HandleWorldChatHordeCommand(ChatHandler * pChat, const char * msg)
    {

        Player* player = pChat->GetSession()->GetPlayer();

        if (!sWorld.GetModuleBoolConfig("World_Chat.Enable", true)) {
            ChatHandler(player).PSendSysMessage("[WC] %sWorld Chat System is disabled.|r", WORLD_CHAT_RED.c_str());
            return true;
        }

        if (!*msg)
            return false;

        uint64 guid = player->GetGUID();

        if (!WorldChat[guid].chat) {
            ChatHandler(player->GetSession()).PSendSysMessage("[WC] %sWorld Chat is disabled. (.chat)|r", WORLD_CHAT_RED.c_str());
            return true;
        }

        char message[1024];


        SessionMap sessions = sWorld.GetAllSessions();

        for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
        {
            if (!itr->second)
                continue;
                
            if (!itr->second->GetPlayer())
            {
                continue;
            }
            if(!itr->second->GetPlayer()->IsInWorld())
            {
                continue;
            }


            Player* target = itr->second->GetPlayer();
            uint64 guid2 = target->GetGUID();

            if (WorldChat[guid2].chat == 1 && (target->GetTeamId() == TEAM_HORDE || target->IsGameMaster()))
            {
                snprintf(message, 1024, "[World][%s][%s%s|r]: %s%s|r", world_chat_GMIcon.c_str(), world_chat_ClassColor[player->getClass() - 1].c_str(), player->GetName(), WORLD_CHAT_WHITE.c_str(), msg);
                ChatHandler(target->GetSession()).PSendSysMessage("%s", message);
            }
        }

        return true;
    }

    static bool HandleWorldChatAllianceCommand(ChatHandler * pChat, const char * msg)
    {

        Player* player = pChat->GetSession()->GetPlayer();

        if (!sWorld.GetModuleBoolConfig("World_Chat.Enable", true)) {
            ChatHandler(player).PSendSysMessage("[WC] %sWorld Chat System is disabled.|r", WORLD_CHAT_RED.c_str());
            return true;
        }

        if (!*msg)
            return false;

        uint64 guid = player->GetGUID();

        if (!WorldChat[guid].chat) {
            ChatHandler(player->GetSession()).PSendSysMessage("[WC] %sWorld Chat is disabled. (.chat)|r", WORLD_CHAT_RED.c_str());
            return true;
        }

        char message[1024];


        SessionMap sessions = sWorld.GetAllSessions();

        for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
        {
            if (!itr->second)
                continue;
                
            if (!itr->second->GetPlayer())
            {
                continue;
            }
            if(!itr->second->GetPlayer()->IsInWorld())
            {
                continue;
            }


            Player* target = itr->second->GetPlayer();
            uint64 guid2 = target->GetGUID();

            if (WorldChat[guid2].chat == 1 && (target->GetTeamId() == TEAM_ALLIANCE || target->IsGameMaster()))
            {
                snprintf(message, 1024, "[World][%s][%s%s|r]: %s%s|r", world_chat_GMIcon.c_str(), world_chat_ClassColor[player->getClass() - 1].c_str(), player->GetName(), WORLD_CHAT_WHITE.c_str(), msg);
                ChatHandler(target->GetSession()).PSendSysMessage("%s",message);
            }
        }

        return true;
    }

    static bool HandleWorldChatOnCommand(ChatHandler* handler, const char* /*msg*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
        uint64 guid = player->GetGUID();

        if (!sWorld.GetModuleBoolConfig("World_Chat.Enable", true)) {
            ChatHandler(player->GetSession()).PSendSysMessage("[WC] %sWorld Chat System is disabled.|r", WORLD_CHAT_RED.c_str());
            return true;
        }

        if (WorldChat[guid].chat) {
            ChatHandler(player->GetSession()).PSendSysMessage("[WC] %sWorld Chat is already visible.|r", WORLD_CHAT_RED.c_str());
            return true;
        }

        WorldChat[guid].chat = 1;

        ChatHandler(player->GetSession()).PSendSysMessage("[WC] %sWorld Chat is now visible.|r", WORLD_CHAT_GREEN.c_str());

        return true;
    };

    static bool HandleWorldChatOffCommand(ChatHandler* handler, const char* /*msg*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
        uint64 guid = player->GetGUID();

        if (!sWorld.GetModuleBoolConfig("World_Chat.Enable", true)) {
            ChatHandler(player->GetSession()).PSendSysMessage("[WC] %sWorld Chat System is disabled.|r", WORLD_CHAT_RED.c_str());
            return true;
        }

        if (!WorldChat[guid].chat) {
            ChatHandler(player->GetSession()).PSendSysMessage("[WC] %sWorld Chat is already hidden.|r", WORLD_CHAT_RED.c_str());
            return true;
        }

        WorldChat[guid].chat = 0;

        ChatHandler(player->GetSession()).PSendSysMessage("[WC] %sWorld Chat is now hidden.|r", WORLD_CHAT_GREEN.c_str());

        return true;
    };

    std::vector<ChatCommand> GetCommands() const override
    {

        static std::vector<ChatCommand> wcCommandTable =
        {
            { "on",      SEC_PLAYER,     false,     &HandleWorldChatOnCommand,      ""},
            { "off",     SEC_PLAYER,     false,    &HandleWorldChatOffCommand,       ""},
            { "",        SEC_PLAYER,     false,    &HandleWorldChatCommand,       ""},

        };

        static std::vector<ChatCommand> commandTable =
        {
            { "chat", SEC_PLAYER, true, NULL , "" , wcCommandTable,},
            { "chath", SEC_MODERATOR, true, &HandleWorldChatHordeCommand , ""},
            { "chata", SEC_MODERATOR, true, &HandleWorldChatAllianceCommand , ""},
        };

        return commandTable;
    }
};

class WorldChat_Announce : public PlayerScript
{
public:

    WorldChat_Announce() : PlayerScript("WorldChat_Announce") {}

    void OnLogin(Player* player, bool firstlogin)
    {
        // Announce Module
        if (sWorld.GetModuleBoolConfig("World_Chat.Enable", true) && sWorld.GetModuleBoolConfig("World_Chat.Announce", true))
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00WorldChat |rmodule");
        }
    }
};

void AddSC_WorldChatScripts()
{
    new WorldChat_Announce();
    new WorldChat_commandscript();
}
