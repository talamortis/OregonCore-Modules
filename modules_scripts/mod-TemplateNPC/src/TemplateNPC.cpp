//Rewritten by LordPsyan (https://www.psyanidegaming.com)
//original script created by ??

#include "TemplateNPC.h"

void sTemplateNPC::LearnPlateMailSpells(Player* player)
{
    switch (player->getClass())
    {
    case CLASS_WARRIOR:
    case CLASS_PALADIN:
    case CLASS_SHAMAN:
    case CLASS_HUNTER:
        player->LearnSpell(MAIL);
        break;
    default:
        break;
    }
}

void sTemplateNPC::ApplyBonus(Player* player, Item* item, EnchantmentSlot slot, uint32 bonusEntry)
{
    if (!item)
        return;

    if (!bonusEntry || bonusEntry == 0)
        return;

    player->ApplyEnchantment(item, slot, false);
    item->SetEnchantment(slot, bonusEntry, 0, 0);
    player->ApplyEnchantment(item, slot, true);
}

void sTemplateNPC::EquipTemplateGear(Player* player)
{
    if (player->GetTeam() == ALLIANCE/* && player->getRace() != RACE_HUMAN*/)
    {
        for (AllianceGearContainer::const_iterator itr = m_AllianceGearContainer.begin(); itr != m_AllianceGearContainer.end(); ++itr)
        {
            if ((*itr)->playerClass == GetClassString(player).c_str() && (*itr)->playerSpec == sTalentsSpec)
            {
                player->EquipNewItem((*itr)->pos, (*itr)->itemEntry, true); // Equip the item and apply enchants and gems
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), PERM_ENCHANTMENT_SLOT, (*itr)->enchant);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), SOCK_ENCHANTMENT_SLOT, (*itr)->socket1);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), SOCK_ENCHANTMENT_SLOT_2, (*itr)->socket2);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), SOCK_ENCHANTMENT_SLOT_3, (*itr)->socket3);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), BONUS_ENCHANTMENT_SLOT, (*itr)->bonusEnchant);
            }
        }
    }
    else if (player->GetTeam() == HORDE)
    {
        for (HordeGearContainer::const_iterator itr = m_HordeGearContainer.begin(); itr != m_HordeGearContainer.end(); ++itr)
        {
            if ((*itr)->playerClass == GetClassString(player).c_str() && (*itr)->playerSpec == sTalentsSpec)
            {
                player->EquipNewItem((*itr)->pos, (*itr)->itemEntry, true); // Equip the item and apply enchants and gems
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), PERM_ENCHANTMENT_SLOT, (*itr)->enchant);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), SOCK_ENCHANTMENT_SLOT, (*itr)->socket1);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), SOCK_ENCHANTMENT_SLOT_2, (*itr)->socket2);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), SOCK_ENCHANTMENT_SLOT_3, (*itr)->socket3);
                ApplyBonus(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, (*itr)->pos), BONUS_ENCHANTMENT_SLOT, (*itr)->bonusEnchant);
            }
        }
    }
}

void sTemplateNPC::LoadAllianceGearContainer()
{
    for (AllianceGearContainer::const_iterator itr = m_AllianceGearContainer.begin(); itr != m_AllianceGearContainer.end(); ++itr)
        delete *itr;

    m_AllianceGearContainer.clear();

    QueryResult* result = CharacterDatabase.PQuery("SELECT playerClass, playerSpec, pos, itemEntry, enchant, socket1, socket2, socket3, bonusEnchant, prismaticEnchant FROM template_npc_alliance;");

    uint32 oldMSTime = getMSTime();
    uint32 count = 0;

    if (!result)
    {
        sLog.outDetail("Loaded 0 'gear templates. DB table `template_npc_alliance` is empty!");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        AllianceGearTemplate* pItem = new AllianceGearTemplate;

        pItem->playerClass = fields[0].GetString();
        pItem->playerSpec = fields[1].GetString();
        pItem->pos = fields[2].GetUInt8();
        pItem->itemEntry = fields[3].GetUInt32();
        pItem->enchant = fields[4].GetUInt32();
        pItem->socket1 = fields[5].GetUInt32();
        pItem->socket2 = fields[6].GetUInt32();
        pItem->socket3 = fields[7].GetUInt32();
        pItem->bonusEnchant = fields[8].GetUInt32();
        pItem->prismaticEnchant = fields[9].GetUInt32();

        m_AllianceGearContainer.push_back(pItem);
        ++count;
    } while (result->NextRow());
    sLog.outDetail("Loaded %u gear templates for Alliances in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void sTemplateNPC::LoadHordeGearContainer()
{
    for (HordeGearContainer::const_iterator itr = m_HordeGearContainer.begin(); itr != m_HordeGearContainer.end(); ++itr)
        delete *itr;

    m_HordeGearContainer.clear();

    QueryResult* result = CharacterDatabase.PQuery("SELECT playerClass, playerSpec, pos, itemEntry, enchant, socket1, socket2, socket3, bonusEnchant, prismaticEnchant FROM template_npc_horde;");

    uint32 oldMSTime = getMSTime();
    uint32 count = 0;

    if (!result)
    {
        sLog.outDetail("Loaded 0 'gear templates. DB table `template_npc_horde` is empty!");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        HordeGearTemplate* pItem = new HordeGearTemplate;

        pItem->playerClass = fields[0].GetString();
        pItem->playerSpec = fields[1].GetString();
        pItem->pos = fields[2].GetUInt8();
        pItem->itemEntry = fields[3].GetUInt32();
        pItem->enchant = fields[4].GetUInt32();
        pItem->socket1 = fields[5].GetUInt32();
        pItem->socket2 = fields[6].GetUInt32();
        pItem->socket3 = fields[7].GetUInt32();
        pItem->bonusEnchant = fields[8].GetUInt32();
        pItem->prismaticEnchant = fields[9].GetUInt32();

        m_HordeGearContainer.push_back(pItem);
        ++count;
    } while (result->NextRow());
    sLog.outDetail("Loaded %u gear templates for Hordes in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

std::string sTemplateNPC::GetClassString(Player* player)
{
    switch (player->getClass())
    {
    case CLASS_PRIEST:       return "Priest";      break;
    case CLASS_PALADIN:      return "Paladin";     break;
    case CLASS_WARRIOR:      return "Warrior";     break;
    case CLASS_MAGE:         return "Mage";        break;
    case CLASS_WARLOCK:      return "Warlock";     break;
    case CLASS_SHAMAN:       return "Shaman";      break;
    case CLASS_DRUID:        return "Druid";       break;
    case CLASS_HUNTER:       return "Hunter";      break;
    case CLASS_ROGUE:        return "Rogue";       break;
    default:
        break;
    }
    return "Unknown"; // Fix warning, this should never happen
}

void sTemplateNPC::ExtractGearTemplateToDB(Player* player, std::string& playerSpecStr)
{
    for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
    {
        Item* equippedItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);

        if (equippedItem)
        {
            if (player->GetTeam() == ALLIANCE/* && player->getRace() != RACE_HUMAN*/)
            {
                CharacterDatabase.PExecute("INSERT INTO template_npc_alliance (`playerClass`, `playerSpec`, `pos`, `itemEntry`, `enchant`, `socket1`, `socket2`, `socket3`, `bonusEnchant`, `prismaticEnchant`) VALUES ('%s', '%s', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u');"
                    , GetClassString(player).c_str(), playerSpecStr.c_str(), equippedItem->GetSlot(), equippedItem->GetEntry(), equippedItem->GetEnchantmentId(PERM_ENCHANTMENT_SLOT),
                    equippedItem->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT), equippedItem->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_2), equippedItem->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_3),
                    equippedItem->GetEnchantmentId(BONUS_ENCHANTMENT_SLOT));
            }
            else if (player->GetTeam() == HORDE)
            {
                CharacterDatabase.PExecute("INSERT INTO template_npc_horde (`playerClass`, `playerSpec`, `pos`, `itemEntry`, `enchant`, `socket1`, `socket2`, `socket3`, `bonusEnchant`, `prismaticEnchant`) VALUES ('%s', '%s', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u');"
                    , GetClassString(player).c_str(), playerSpecStr.c_str(), equippedItem->GetSlot(), equippedItem->GetEntry(), equippedItem->GetEnchantmentId(PERM_ENCHANTMENT_SLOT),
                    equippedItem->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT), equippedItem->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_2), equippedItem->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_3),
                    equippedItem->GetEnchantmentId(BONUS_ENCHANTMENT_SLOT));
            }
        }
    }
}

bool sTemplateNPC::CanEquipTemplate(Player* player, std::string& playerSpecStr)
{
    if (player->GetTeam() == ALLIANCE/* && player->getRace() != RACE_HUMAN*/)
    {
        QueryResult* result = CharacterDatabase.PQuery("SELECT playerClass, playerSpec FROM template_npc_alliance "
            "WHERE playerClass = '%s' AND playerSpec = '%s';", GetClassString(player).c_str(), playerSpecStr.c_str());

        if (!result)
            return false;
    }
    else if (player->GetTeam() == HORDE)
    {
        QueryResult* result = CharacterDatabase.PQuery("SELECT playerClass, playerSpec FROM template_npc_horde "
            "WHERE playerClass = '%s' AND playerSpec = '%s';", GetClassString(player).c_str(), playerSpecStr.c_str());

        if (!result)
            return false;
    }
    return true;
}

class TemplateNPC : public CreatureScript
{
public:
    TemplateNPC() : CreatureScript("TemplateNPC") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if(!sWorld.GetModuleBoolConfig("TemplateNPC.Enable", true))
        {
        switch (player->getClass())
        {
        case CLASS_PRIEST:
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_holy_wordfortitude:30|t|r Use Discipline Spec", GOSSIP_SENDER_MAIN, 0);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_holy_holybolt:30|t|r Use Holy Spec", GOSSIP_SENDER_MAIN, 1);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_shadow_shadowwordpain:30|t|r Use Shadow Spec", GOSSIP_SENDER_MAIN, 2);
        }
        break;
        case CLASS_PALADIN:
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_holy_holybolt:30|t|r Use Holy Spec", GOSSIP_SENDER_MAIN, 3);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_holy_devotionaura:30|t|r Use Protection Spec", GOSSIP_SENDER_MAIN, 4);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_holy_auraoflight:30|t|r Use Retribution Spec", GOSSIP_SENDER_MAIN, 5);
        }
        break;
        case CLASS_WARRIOR:
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_warrior_innerrage:30|t|r Use Fury Spec", GOSSIP_SENDER_MAIN, 6);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_rogue_eviscerate:30|t|r Use Arms Spec", GOSSIP_SENDER_MAIN, 7);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_warrior_defensivestance:30|t|r Use Protection Spec", GOSSIP_SENDER_MAIN, 8);
        }
        break;
        case CLASS_MAGE:
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_holy_magicalsentry:30|t|r Use Arcane Spec", GOSSIP_SENDER_MAIN, 9);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_fire_flamebolt:30|t|r Use Fire Spec", GOSSIP_SENDER_MAIN, 10);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_frost_frostbolt02:30|t|r Use Frost Spec", GOSSIP_SENDER_MAIN, 11);
        }
        break;
        case CLASS_WARLOCK:
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_shadow_deathcoil:30|t|r Use Affliction Spec", GOSSIP_SENDER_MAIN, 12);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_shadow_metamorphosis:30|t|r Use Demonology Spec", GOSSIP_SENDER_MAIN, 13);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_shadow_rainoffire:30|t|r Use Destruction Spec", GOSSIP_SENDER_MAIN, 14);
        }
        break;
        case CLASS_SHAMAN:
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_nature_lightning:30|t|r Use Elemental Spec", GOSSIP_SENDER_MAIN, 15);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_nature_lightningshield:30|t|r Use Enhancement Spec", GOSSIP_SENDER_MAIN, 16);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_nature_magicimmunity:30|t|r Use Restoration Spec", GOSSIP_SENDER_MAIN, 17);
        }
        break;
        case CLASS_DRUID:
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_nature_starfall:30|t|r Use Ballance Spec", GOSSIP_SENDER_MAIN, 18);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_racial_bearform:30|t|r Use Feral Spec", GOSSIP_SENDER_MAIN, 19);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\spell_nature_healingtouch:30|t|r Use Restoration Spec", GOSSIP_SENDER_MAIN, 20);
        }
        break;
        case CLASS_HUNTER:
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_marksmanship:30|t|r Use Markmanship Spec", GOSSIP_SENDER_MAIN, 21);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_hunter_beasttaming:30|t|r Use TemplateNPCy Spec", GOSSIP_SENDER_MAIN, 22);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_Hunter_swiftstrike:30|t|r Use Survival Spec", GOSSIP_SENDER_MAIN, 23);
        }
        break;
        case CLASS_ROGUE:
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_rogue_eviscerate:30|t|r Use Assasination Spec", GOSSIP_SENDER_MAIN, 24);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_backstab:30|t|r Use Combat Spec", GOSSIP_SENDER_MAIN, 25);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "|cff00ff00|TInterface\\icons\\ability_stealth:30|t|r Use Subtlety Spec", GOSSIP_SENDER_MAIN, 26);
        }
        break;
        }
        player->SEND_GOSSIP_MENU(60025, creature->GetGUID());
        return true;
        }
    }

    void EquipFullTemplateGear(Player* player, std::string& playerSpecStr) // Merge
    {
        if (sTemplateNpcMgr->CanEquipTemplate(player, playerSpecStr) == false)
        {
            player->GetSession()->SendAreaTriggerMessage("There's no templates for %s specialization yet.", playerSpecStr.c_str());
            return;
        }

        // Don't let players to use Template feature while wearing some gear
        for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
        {
            if (Item* haveItemEquipped = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            {
                if (haveItemEquipped)
                {
                    player->GetSession()->SendAreaTriggerMessage("You need to remove all your equipped items in order to use this feature!");
                    player->CLOSE_GOSSIP_MENU();
                    return;
                }
            }
        }

        // Don't let players to use Template feature after spending some talent points
        if (player->GetFreeTalentPoints() < 61)
        {
            player->GetSession()->SendAreaTriggerMessage("You have already spent some talent points. You need to reset your talents first!");
            player->CLOSE_GOSSIP_MENU();
            return;
        }

        //sTemplateNpcMgr->LearnTemplateTalents(player);
        sTemplateNpcMgr->EquipTemplateGear(player);
        sTemplateNpcMgr->LearnPlateMailSpells(player);

        LearnWeaponSkills(player);

        player->CastSpell(player,46642,true);
        player->GetSession()->SendAreaTriggerMessage("Successfuly equipped %s %s template!", playerSpecStr.c_str(), sTemplateNpcMgr->GetClassString(player).c_str());
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();

        if (!player || !creature)
            return true;

        switch (uiAction)
        {
        case 0: // Use Discipline Priest Spec
            sTemplateNpcMgr->sTalentsSpec = "Discipline";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 1: // Use Holy Priest Spec
            sTemplateNpcMgr->sTalentsSpec = "Holy";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 2: // Use Shadow Priest Spec
            sTemplateNpcMgr->sTalentsSpec = "Shadow";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 3: // Use Holy Paladin Spec
            sTemplateNpcMgr->sTalentsSpec = "Holy";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 4: // Use Protection Paladin Spec
            sTemplateNpcMgr->sTalentsSpec = "Protection";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 5: // Use Retribution Paladin Spec
            sTemplateNpcMgr->sTalentsSpec = "Retribution";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 6: // Use Fury Warrior Spec
            sTemplateNpcMgr->sTalentsSpec = "Fury";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 7: // Use Arms Warrior Spec
            sTemplateNpcMgr->sTalentsSpec = "Arms";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 8: // Use Protection Warrior Spec
            sTemplateNpcMgr->sTalentsSpec = "Protection";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 9: // Use Arcane Mage Spec
            sTemplateNpcMgr->sTalentsSpec = "Arcane";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 10: // Use Fire Mage Spec
            sTemplateNpcMgr->sTalentsSpec = "Fire";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 11: // Use Frost Mage Spec
            sTemplateNpcMgr->sTalentsSpec = "Frost";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 12: // Use Affliction Warlock Spec
            sTemplateNpcMgr->sTalentsSpec = "Affliction";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 13: // Use Demonology Warlock Spec
            sTemplateNpcMgr->sTalentsSpec = "Demonology";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 14: // Use Destruction Warlock Spec
            sTemplateNpcMgr->sTalentsSpec = "Destruction";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 15: // Use Elemental Shaman Spec
            sTemplateNpcMgr->sTalentsSpec = "Elemental";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 16: // Use Enhancement Shaman Spec
            sTemplateNpcMgr->sTalentsSpec = "Enhancement";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 17: // Use Restoration Shaman Spec
            sTemplateNpcMgr->sTalentsSpec = "Restoration";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 18: // Use Ballance Druid Spec
            sTemplateNpcMgr->sTalentsSpec = "Ballance";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 19: // Use Feral Druid Spec
            sTemplateNpcMgr->sTalentsSpec = "Feral";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 20: // Use Restoration Druid Spec
            sTemplateNpcMgr->sTalentsSpec = "Restoration";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 21: // Use Marksmanship Hunter Spec
            sTemplateNpcMgr->sTalentsSpec = "Marksmanship";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 22: // Use TemplateNPCy Hunter Spec
            sTemplateNpcMgr->sTalentsSpec = "TemplateNPCy";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 23: // Use Survival Hunter Spec
            sTemplateNpcMgr->sTalentsSpec = "Survival";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 24: // Use Assassination Rogue Spec
            sTemplateNpcMgr->sTalentsSpec = "Assassination";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 25: // Use Combat Rogue Spec
            sTemplateNpcMgr->sTalentsSpec = "Combat";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        case 26: // Use Subtlety Rogue Spec
            sTemplateNpcMgr->sTalentsSpec = "Subtlety";
            EquipFullTemplateGear(player, sTemplateNpcMgr->sTalentsSpec);
            player->CLOSE_GOSSIP_MENU();
            break;

        default: // Just in case
            player->GetSession()->SendAreaTriggerMessage("Something went wrong in the code. Please contact the administrator.");
            break;
        }
        player->UpdateSkillsForLevel();
        return true;
    }
};


class TemplateNPC_Announce : public PlayerScript
{
public:

    TemplateNPC_Announce() : PlayerScript("TemplateNPC_Announce") {}

    void OnLogin(Player* player, bool firstlogin)
    {
        // Announce Module
        if (sWorld.GetModuleBoolConfig("TemplateNPC.Enable", true) && sWorld.GetModuleBoolConfig("TemplateNPC.Announce", true))
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00TemplateNPC |rmodule by |cff4CFF00LordPsyan");
        }
    }
};

void AddSC_TemplateNPC()
{
    new TemplateNPC();
    new TemplateNPC_Announce();
}