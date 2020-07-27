#include "ScriptMgr.h"
#include "Config/Config.h"
#include "Player.h"
#include "World.h"
#include "Chat.h"

class modsample : public PlayerScript
{
public:
    modsample() : PlayerScript("modsample") { }

    void OnLogin(Player* player, bool firstLogin) override
    {
        ChatHandler(player->GetSession()).SendSysMessage("Dynamic Module Is working!!");
        std::string mod = sWorld.GetModuleStringConfig("modsample.stringtest");
        uint32 number = sWorld.GetModuleIntConfig("modsample.intTest", 10032);

        if (sWorld.GetModuleBoolConfig("modsample.enableHelloWorld", "1"))
        {
            ChatHandler(player->GetSession()).SendSysMessage("GetModuleBoolConfig get true");
            ChatHandler(player->GetSession()).SendSysMessage(mod.c_str());
            player->AddAura(number, player);
        }
        else
            ChatHandler(player->GetSession()).SendSysMessage("GetModuleBoolConfig get false");
    }
};

void Addmod_sampleScripts()
{
    new modsample();
}