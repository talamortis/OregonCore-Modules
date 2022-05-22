#include "WorldSession.h"
#include "Player.h"
#include "ChannelMgr.h"
#include "Channel.h"
#include "Chat.h"
#include "ScriptMgr.h"
#include "SocialMgr.h"
#include "Utilities/DataMap.h"
#include <string>

std::vector<std::string> PlayerIP;

class mod_multilog : public PlayerScript
{
public:
    mod_multilog() : PlayerScript("mod_multilog") { }

    void OnLogin(Player* player, bool firstLogin)
    {
        if (sWorld.GetModuleBoolConfig("Disallow.Multiple.Client", false))
        {
            if (sWorld.GetModuleBoolConfig("Disallow.Multiple.Client.Announce", true))
            {
                ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00DisallowMultipleClient |rmodule.");
            }

            SessionMap sessions = sWorld.GetAllSessions();
            for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
            {
                if (Player* login = itr->second->GetPlayer())
                {
                    if (player != login)
                    {
                        // If Remote Address matches, remove the player from the world
                        if (player->GetSession()->GetRemoteAddress() == login->GetSession()->GetRemoteAddress())
                            player->GetSession()->KickPlayer();
                    }
                }
            }
        }
    }
};

void Addmod_multilogScripts()
{
    new mod_multilog();
}