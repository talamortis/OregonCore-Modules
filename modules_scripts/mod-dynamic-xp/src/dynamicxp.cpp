/*
Credits
Script reworked by Micrah/Milestorme and Poszer (Poszer is the Best)
Module Created by Micrah/Milestorme
Original Script from AshmaneCore https://github.com/conan513 Single Player Project 
*/
#include "SystemConfig.h"
#include "ScriptPCH.h"
#include "Player.h"
#include "Chat.h"
#include "World.h"
#include "Common.h"


class spp_dynamic_xp_rate : public PlayerScript
{
    public:
        spp_dynamic_xp_rate() : PlayerScript("spp_dynamic_xp_rate") { };

        void OnLogin(Player* player, bool /*First Login*/) override
       {
            if (sWorld.GetModuleBoolConfig("Dynamic.XP.Rate.Announce", true))
               {
               ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Level Dynamic XP |rmodule by |cff4CFF00Milestorme.");
               }
       }
        void OnGiveXP(Player* player, uint32& amount, Unit* /*victim*/) override
       {
        if (sWorld.GetModuleBoolConfig("Dynamic.XP.Rate", true))
        
	   {
           if (player->getLevel() <= 9)
            amount *= sWorld.GetModuleIntConfig("Dynamic.XP.Rate.1-9", 1);

           else if (player->getLevel() <= 19)
            amount *= sWorld.GetModuleIntConfig("Dynamic.XP.Rate.10-19", 2);

           else if (player->getLevel() <= 29)
            amount *= sWorld.GetModuleIntConfig("Dynamic.XP.Rate.20-29", 3);

           else if (player->getLevel() <= 39)
            amount *= sWorld.GetModuleIntConfig("Dynamic.XP.Rate.30-39", 4);

           else if (player->getLevel() <= 49)
            amount *= sWorld.GetModuleIntConfig("Dynamic.XP.Rate.40-49", 5);

           else if (player->getLevel() <= 59)
            amount *= sWorld.GetModuleIntConfig("Dynamic.XP.Rate.50-59", 6);

           else if (player->getLevel() <= 69)
            amount *= sWorld.GetModuleIntConfig("Dynamic.XP.Rate.60-69", 7);
        }

     }
    
};

void AddSC_dynamic_xp_rate()
{
    new spp_dynamic_xp_rate();
}
