//Rewritten by LordPsyan (https://www.psyanidegaming.com)
//original script created by ??
#include "ScriptPCH.h"
#include "Config.h"
#include "World.h"
#include "Chat.h"

class Summon : public ItemScript
{
        public:
                Summon() : ItemScript("Summon")
                {
                }
                bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& /*targets*/)
                {
   uint32 sSpell = sWorld.GetModuleIntConfig("Summon.SpellId", 40162); //Icerune Visual Spell when used
   uint32 charEntry = sWorld.GetModuleIntConfig("Summon.CreatureEntry", 100000);
   if (pPlayer->IsInCombat())
   {
        pPlayer->GetSession()->SendNotification("You are in combat.");
   }
  
   else if (pPlayer->isMoving())
   {
        pPlayer->GetSession()->SendNotification("You can not do that while moving.");
   }
  
   else if (pPlayer->FindNearestCreature(charEntry, 40) || pPlayer->FindNearestCreature(charEntry, 8))
   {
        pPlayer->GetSession()->SendNotification("Flaming Legion Vortex can not be Summoned right now. Another Vortex is nearby.");
   }
  
   else
   {
        float x, y, z, o = pPlayer->GetOrientation();
  
        if (o >= 3.141592)
         o = o - 3.141592;
        else
         o = o + 3.141592;
        pPlayer->GetClosePoint(x, y, z, pPlayer->GetObjectSize());
        Creature* pCreature = pPlayer->SummonCreature(charEntry, x, y, z+1.2, o, TEMPSUMMON_TIMED_DESPAWN, 80000);
        pCreature->CastSpell(pCreature, sSpell, false);
        return false;
   }
  return false;
                }
};

class Summon_Announce : public PlayerScript
{
public:

    Summon_Announce() : PlayerScript("Summon_Announce") {}

    void OnLogin(Player* player, bool firstlogin)
    {
        // Announce Module
        if (sWorld.GetModuleBoolConfig("Summon.Enable", true) && sWorld.GetModuleBoolConfig("Summon.Announce", true))
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Summoning Item |rmodule by |cff4CFF00LordPsyan");
        }
    }
};

void AddSC_Summon()
{
        new Summon();
        new Summon_Announce();
}
