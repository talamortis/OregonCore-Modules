#include "ScriptMgr.h"
#include <time.h>

time_t t = time(NULL);
tm *now = localtime(&t);

class DoubleXpWeekend : public PlayerScript
{
public:
    DoubleXpWeekend() : PlayerScript("DoubleXpWeekend") {}

    bool Enabled = sWorld.GetModuleBoolConfig("XPWeekend.Enabled", true);
    uint32 xpAmount = sWorld.GetModuleIntConfig("XPWeekend.xpAmount", 2);

        void OnLogin(Player* player, bool firstLogin)
        {
            // Announce to the player that the XP weekend is happeneing.
            if (!Enabled)
                return;

            if (now->tm_wday == 5 /*Friday*/ || now->tm_wday == 6 /*Satureday*/ || now->tm_wday == 0/*Sunday*/)
                ChatHandler(player->GetSession()).PSendSysMessage("Its the Weekend! Your XP rate has been set to: %u", xpAmount);
            else
                ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00XPWeekend |rmodule");
        }

        void OnGiveXP(Player* p, uint32& amount, Unit* victim) override
        {
            if (!Enabled)
                return;

            if (now->tm_wday == 5 /*Friday*/ || now->tm_wday == 6 /*Satureday*/ || now->tm_wday == 0/*Sunday*/ && now->tm_hour >= 0)
                amount *= xpAmount;
            else
                amount == amount;
        }
};

void AddDoubleXPScripts()
{
    new DoubleXpWeekend();
}