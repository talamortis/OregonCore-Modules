#include "Player.h"
#include "ScriptMgr.h"
#include "WorldSession.h"
#include "ObjectMgr.h"
#include "Chat.h"

struct SystemInfo
{
    uint32 KillStreak;
    uint32 LastGUIDKill;
};

static std::map<uint32, SystemInfo> KillingStreak;

class killstreak : public PlayerScript
{
public:
    killstreak() : PlayerScript("killstreak") { }


    void OnPVPKill(Player *Killer, Player *Victim)
    {
        if (!Killer->InBattleground() && !Victim->InBattleground())
        {
            uint32 kGUID;
            uint32 vGUID;
            kGUID = Killer->GetGUID();
            vGUID = Victim->GetGUID();
            if (kGUID == vGUID)
                return;

            if (KillingStreak[kGUID].LastGUIDKill == vGUID)
                return;

            if (Killer->GetSession()->GetRemoteAddress() == Victim->GetSession()->GetRemoteAddress())
                return;

            if (!Killer->isHonorOrXPTarget(Victim))
                return;

            KillingStreak[kGUID].KillStreak++;
            KillingStreak[vGUID].KillStreak = 0;
            KillingStreak[kGUID].LastGUIDKill = vGUID;
            KillingStreak[vGUID].LastGUIDKill = 0;

            //QueryResult result = CharacterDatabase.PQuery("select killer");

            switch (KillingStreak[kGUID].KillStreak)
            {
                char msg[500];

            case 15:
                Killer->SetHonorPoints(Killer->GetHonorPoints() + 50);
                sprintf(msg, "%s killed %s, Stunning killingspree of 15 victims! ", Killer->GetName(), Victim->GetName());
                sWorld.SendServerMessage(SERVER_MSG_STRING, msg);
                break;

            case 30:
                Killer->SetHonorPoints(Killer->GetHonorPoints() + 50);
                sprintf(msg, "%s killed %s, Unbelievable killingspree of 30 victims! ", Killer->GetName(), Victim->GetName());
                sWorld.SendServerMessage(SERVER_MSG_STRING, msg);
                break;

            case 50:
                Killer->SetHonorPoints(Killer->GetHonorPoints() + 50);
                sprintf(msg, "%s killed %s, Outrageous killingspree of 50 victims! ", Killer->GetName(), Victim->GetName());
                sWorld.SendServerMessage(SERVER_MSG_STRING, msg);
                KillingStreak[kGUID].KillStreak = 0;
                break;
            }
        }
    }
};

void AddSC_killstreakScripts()
{
    new killstreak();
}