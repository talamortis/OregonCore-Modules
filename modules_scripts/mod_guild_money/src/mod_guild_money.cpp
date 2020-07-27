#include "ScriptMgr.h"
#include "Configuration/Config.h"
#include "Player.h"
#include "World.h"
#include "Chat.h"
#include "Guild.h"

class mod_guild_money : public PlayerScript
{
public:
    mod_guild_money() : PlayerScript("guild_money") { }

    void OnLootMoney(Player* player, uint32 amount)
    {
        if (!sWorld.GetModuleBoolConfig("Guild.Reward.Money.Enabled", true))
            return;

        DepositMoney(player, amount);
    }

    void DepositMoney(Player* player, uint32 money)
    {

        Guild* guild = sObjectMgr.GetGuildById(player->GetGuildId());

        if (!guild)
            return;

        Group* grp = player->GetGroup();

        if (grp ? money = (money / sWorld.GetModuleIntConfig("Guild.Reward.Money.Group", 3)) :  (money = money / sWorld.GetModuleIntConfig("Guild.Reward.Money.Solo", 2)))
        {
            guild->SetBankMoney(guild->GetGuildBankMoney() + money);
            guild->LogBankEvent(GUILD_BANK_LOG_DEPOSIT_MONEY, uint8(0), player->GetGUIDLow(), money);

            if (sWorld.GetModuleBoolConfig("Guild.Reward.Notify.Player", true))
                ChatHandler(player).PSendSysMessage("You Deposit %u to %s", money, guild->GetName().c_str());
        }
    }
};

void Addguild_moneyScripts()
{
    new mod_guild_money();
}