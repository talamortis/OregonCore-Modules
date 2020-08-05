#include "ScriptMgr.h"
#include "Configuration/Config.h"
#include "Player.h"
#include "World.h"
#include "Chat.h"

class modLoginAnnounce : public PlayerScript
{
public:
    modLoginAnnounce() : PlayerScript("LoginAnnounce") { }

    void OnLogin(Player* player, bool firstLogin) override
    {

        if (!sWorld.GetModuleBoolConfig("LoginAnnounce.Enable", true))
            return;

        std::ostringstream ss;

        if (player->GetTeam() == ALLIANCE)
        {
            ss << "|cff3DAEFF[ Login Announcer ]|cffFFD800 : Player|cff4CFF00 " << player->GetName() << " |cffFFD800Is Online. This Player is|cff0026FF Alliance";
            sWorld.SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());
        }
        else
        {
            ss << "|cff3DAEFF[ Login Announcer ]|cffFFD800 : Player|cff4CFF00 " << player->GetName() << " |cffFFD800Is Online. This Player is|cffFF0000 Horde";
            sWorld.SendServerMessage(SERVER_MSG_STRING, ss.str().c_str());
        }
    }
};

void Addmod_LoginAnnounceScripts()
{
    new modLoginAnnounce();
}