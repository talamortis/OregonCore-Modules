#include "ScriptPCH.h"
#include "mod_customize.h"
#include <cstring>

struct npc_customizer : public CreatureScript
{
public:
    npc_customizer() : CreatureScript("npc_customizer") {}

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        for (uint32 i = 1; i <= 3; i++)
        {
            QueryResult* type = WorldDatabase.PQuery("SELECT `text`, `action` FROM `npc_customer` WHERE `type` = %u AND `active` = 1", i);
            if (type)
            {
                Field *fields = type->Fetch();
                pPlayer->ADD_GOSSIP_ITEM(0, fields[0].GetString(), GOSSIP_SENDER_MAIN, fields[1].GetInt32());
            }
        }

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
        return true;
    }

    void ChangeEffect(Player *player)
    {
        // First Send update to player, so most recent datas are up
        player->SendUpdateToPlayer(player);

        // Force client to reload this player, so changes are visible
        WorldPacket data(SMSG_FORCE_DISPLAY_UPDATE, 8);
        data << player->GetGUID();
        player->SendMessageToSet(&data, true);

        // Do some visual effect ( Vanish visual spell )
        player->CastSpell(player, 24222, true);
    }

    void SelectSkin(Player* player, uint8 change)
    {
        uint8 max = maxSkinColor[player->getRace()];

        uint8 current = player->GetByteValue(PLAYER_BYTES, 0);

        current += change;

        if (current > max)
            current = 0;
        else if (current < 0)
            current = max;

        player->SetByteValue(PLAYER_BYTES, 0, current);
        ChangeEffect(player);
    }

    void SelectFace(Player* player, uint8 change)
    {
        uint8 max = maxFaceStyles[player->getRace()].maxMale;
        if (player->getGender() == GENDER_FEMALE)
            max = maxHairStyles[player->getRace()].maxFemale;

        uint8 current = player->GetByteValue(PLAYER_BYTES, 1);

        current += change;

        if (current > max)
            current = 0;
        else if (current < 0)
            current = max;

        player->SetByteValue(PLAYER_BYTES, 1, current);
        ChangeEffect(player);
    }

    void SelectHairStyle(Player *player, uint8 change)
    {
        uint8 max = maxHairStyles[player->getRace()].maxMale;
        if (player->getGender() == GENDER_FEMALE)
            max = maxHairStyles[player->getRace()].maxFemale;

        int current = player->GetByteValue(PLAYER_BYTES, 2);

        current += change;

        if (current > max)
            current = 0;
        else if (current < 0)
            current = max;

        player->SetByteValue(PLAYER_BYTES, 2, current);
        ChangeEffect(player);
    }

    void SelectHairColor(Player *player, uint8 change)
    {
        uint8 max = maxHairColor[player->getRace()];
        uint8 current = player->GetByteValue(PLAYER_BYTES, 3);

        current += change;

        if (current > max)
            current = 0;
        else if (current < 0)
            current = max;

        player->SetByteValue(PLAYER_BYTES, 3, current);
        //player->SetUInt32Value(PLAYER_BYTES_2, (current | (0x00 << 8) | (0x00 << 16) | (0x02 << 24)));
        ChangeEffect(player);
    }

    void SelectFacialFeature(Player *player, uint8 change)
    {
        uint8 max = maxFacialFeatures[player->getRace()].maxMale;
        if (player->getGender() == GENDER_FEMALE)
            max = maxFacialFeatures[player->getRace()].maxFemale;

        int current = player->GetByteValue(PLAYER_BYTES_2, 0);

        current += change;

        if (current > max)
            current = 0;
        else if (current < 0)
            current = max;

        player->SetByteValue(PLAYER_BYTES_2, 0, current);
        ChangeEffect(player);
    }

    void ReplaceSpells(Player* player,uint8 oldrace,  uint8 newrace)
    {
        bool native;

        if (player->getFactionForRace(oldrace) == player->getFactionForRace(newrace))
            native = true;
        else
            native = false;

        auto spells = UNORDERED_MAP<uint32, bool>();

        for (auto& i : sObjectMgr.GetPlayerInfo(native ? newrace : oldrace, player->getClass())->spell)
            if (auto spell = sSpellStore.LookupEntry(i.first))
                if (!sWorld.getConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHAT))
                    spells[spell->Id] = false;
                else if (spell->Effect[0] != SPELL_EFFECT_LANGUAGE && !sWorld.getConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHAT))
                    spells[spell->Id] = false;

        for (auto& i : sObjectMgr.GetPlayerInfo(native ? oldrace : newrace, player->getClass())->spell)
            if (auto spell = sSpellStore.LookupEntry(i.first))
                if ( !sWorld.getConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHAT))
                    spells[spell->Id] = true;
                else if (spell->Effect[0] != SPELL_EFFECT_LANGUAGE && !sWorld.getConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHAT))
                    spells[spell->Id] = true;


        for (auto& i : spells)
        {
            if (i.second)
                player->LearnSpell(i.first);
            else
                player->RemoveSpell(i.first);
        }
        sWorld.InvalidatePlayerDataToAllClient(player->GetGUID());
        player->GetSession()->LogoutPlayer(true);

    }

    void SetRace(Player* player,uint8 oldrace,  uint8 race)
    {

        oldrace = player->getRace();

        player->SetByteValue(UNIT_FIELD_BYTES_0, 0, race);

        PlayerInfo const* info = sObjectMgr.GetPlayerInfo(player->getRace(), player->getClass());

        player->SetObjectScale(1.0f);

        uint8 gender = player->getGender();

        switch (gender)
        {
        case GENDER_FEMALE:
            player->SetDisplayId(info->displayId_f);
            player->SetNativeDisplayId(info->displayId_f);
            break;
        case GENDER_MALE:
            player->SetDisplayId(info->displayId_m);
            player->SetNativeDisplayId(info->displayId_m);
            break;
        default:
            sLog.outError("Invalid gender %u for player", gender);
            return;
        }

        ReplaceSpells(player, oldrace,  race);
    }

    bool SendDefaultMenu(Player* pPlayer, Creature* pCreature, uint32 uiAction)
    {
        // Not allowed if in combat.
        if (pPlayer->IsInCombat())
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->MonsterSay("You are in combat!", LANG_UNIVERSAL, NULL);
            return false;
        }
        switch (uiAction)
        {
        case 1001:
        {
            QueryResult* type1 = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 1");
            Field *fields1 = type1->Fetch();
            uint32 gold = fields1[0].GetUInt32();
            uint32 honor = fields1[1].GetUInt32();
            uint32 ap = fields1[2].GetUInt32();
            uint32 itemid = fields1[3].GetUInt32();
            uint32 itemcount = fields1[4].GetUInt32();
            if (pPlayer->GetMoney() < gold * 10000)
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterWhisper("You have not enough Gold", pPlayer->GetGUID());
                return false;
            }
            if (pPlayer->GetHonorPoints() < honor)
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterWhisper("You have not enough Honor Points", pPlayer->GetGUID());
                return false;
            }
            if (pPlayer->GetArenaPoints() < ap)
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterWhisper("You have not enough Arena Points", pPlayer->GetGUID());
                return false;
            }
            if (pPlayer->HasItemCount(itemid, itemcount))
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterWhisper("You have not enough Items", pPlayer->GetGUID());
                return false;
            }

            switch (pPlayer->getClass())
            {
            case CLASS_WARRIOR:
            {
                pPlayer->ADD_GOSSIP_ITEM(0, "To Human", GOSSIP_SENDER_MAIN, 2001);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Orc", GOSSIP_SENDER_MAIN, 2002);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Dwarf", GOSSIP_SENDER_MAIN, 2003);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Night Elf", GOSSIP_SENDER_MAIN, 2004);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Unded", GOSSIP_SENDER_MAIN, 2005);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Tauren", GOSSIP_SENDER_MAIN, 2006);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Gnome", GOSSIP_SENDER_MAIN, 2007);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Troll", GOSSIP_SENDER_MAIN, 2008);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Draenei", GOSSIP_SENDER_MAIN, 2010);
            }
            break;
            case CLASS_PALADIN:
            {
                pPlayer->ADD_GOSSIP_ITEM(0, "To Human", GOSSIP_SENDER_MAIN, 2001);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Dwarf", GOSSIP_SENDER_MAIN, 2003);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Blood Elf", GOSSIP_SENDER_MAIN, 2009);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Draenei", GOSSIP_SENDER_MAIN, 2010);
            }
            break;
            case CLASS_HUNTER:
            {
                pPlayer->ADD_GOSSIP_ITEM(0, "To Orc", GOSSIP_SENDER_MAIN, 2002);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Dwarf", GOSSIP_SENDER_MAIN, 2003);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Night Elf", GOSSIP_SENDER_MAIN, 2004);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Tauren", GOSSIP_SENDER_MAIN, 2006);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Troll", GOSSIP_SENDER_MAIN, 2008);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Blood Elf", GOSSIP_SENDER_MAIN, 2009);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Draenei", GOSSIP_SENDER_MAIN, 2010);
            }
            break;
            case CLASS_ROGUE:
            {
                pPlayer->ADD_GOSSIP_ITEM(0, "To Human", GOSSIP_SENDER_MAIN, 2001);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Orc", GOSSIP_SENDER_MAIN, 2002);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Dwarf", GOSSIP_SENDER_MAIN, 2003);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Night Elf", GOSSIP_SENDER_MAIN, 2004);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Unded", GOSSIP_SENDER_MAIN, 2005);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Gnome", GOSSIP_SENDER_MAIN, 2007);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Troll", GOSSIP_SENDER_MAIN, 2008);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Blood Elf", GOSSIP_SENDER_MAIN, 2009);
            }
            break;
            case CLASS_PRIEST:
            {
                pPlayer->ADD_GOSSIP_ITEM(0, "To Human", GOSSIP_SENDER_MAIN, 2001);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Dwarf", GOSSIP_SENDER_MAIN, 2003);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Night Elf", GOSSIP_SENDER_MAIN, 2004);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Unded", GOSSIP_SENDER_MAIN, 2005);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Troll", GOSSIP_SENDER_MAIN, 2008);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Blood Elf", GOSSIP_SENDER_MAIN, 2009);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Draenei", GOSSIP_SENDER_MAIN, 2010);
            }
            break;
            case CLASS_SHAMAN:
            {
                pPlayer->ADD_GOSSIP_ITEM(0, "To Orc", GOSSIP_SENDER_MAIN, 2002);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Tauren", GOSSIP_SENDER_MAIN, 2006);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Troll", GOSSIP_SENDER_MAIN, 2008);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Draenei", GOSSIP_SENDER_MAIN, 2010);
            }
            break;
            case CLASS_MAGE:
            {
                pPlayer->ADD_GOSSIP_ITEM(0, "To Human", GOSSIP_SENDER_MAIN, 2001);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Unded", GOSSIP_SENDER_MAIN, 2005);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Gnome", GOSSIP_SENDER_MAIN, 2007);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Troll", GOSSIP_SENDER_MAIN, 2008);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Blood Elf", GOSSIP_SENDER_MAIN, 2009);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Draenei", GOSSIP_SENDER_MAIN, 2010);
            }
            break;
            case CLASS_WARLOCK:
            {
                pPlayer->ADD_GOSSIP_ITEM(0, "To Human", GOSSIP_SENDER_MAIN, 2001);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Orc", GOSSIP_SENDER_MAIN, 2002);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Unded", GOSSIP_SENDER_MAIN, 2005);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Gnome", GOSSIP_SENDER_MAIN, 2007);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Blood Elf", GOSSIP_SENDER_MAIN, 2009);
            }
            break;
            case CLASS_DRUID:
            {
                pPlayer->ADD_GOSSIP_ITEM(0, "To Night Elf", GOSSIP_SENDER_MAIN, 2004);
                pPlayer->ADD_GOSSIP_ITEM(0, "To Tauren", GOSSIP_SENDER_MAIN, 2006);
            }
            break;
            }

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());

        }
        break;
        case 1002:
        {
            QueryResult* type2 = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 2");
            Field *fields2 = type2->Fetch();
            uint32 gold = fields2[0].GetUInt32();
            uint32 honor = fields2[1].GetUInt32();
            uint32 ap = fields2[2].GetUInt32();
            uint32 itemid = fields2[3].GetUInt32();
            uint32 itemcount = fields2[4].GetUInt32();
            if (pPlayer->GetMoney() < gold * 10000)
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterWhisper("You have not enough Gold", pPlayer->GetGUID());
                return false;
            }
            if (pPlayer->GetHonorPoints() < honor)
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterWhisper("You have not enough Honor Points", pPlayer->GetGUID());
                return false;
            }
            if (pPlayer->GetArenaPoints() < ap)
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterWhisper("You have not enough Arena Points", pPlayer->GetGUID());
                return false;
            }
            if (pPlayer->HasItemCount(itemid, itemcount))
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterWhisper("You have not enough Items", pPlayer->GetGUID());
                return false;
            }

            if (!pPlayer->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM))
                pPlayer->ToggleFlag(PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM);
            pPlayer->ADD_GOSSIP_ITEM(0, "Change Gender.", GOSSIP_SENDER_MAIN, 1501);
            pPlayer->ADD_GOSSIP_ITEM(0, "Change Skin Color.", GOSSIP_SENDER_MAIN, 1502);
            pPlayer->ADD_GOSSIP_ITEM(0, "Change Face.", GOSSIP_SENDER_MAIN, 1503);
            switch (pPlayer->getRace())
            {
            case RACE_HUMAN:
            case RACE_ORC:
            case RACE_DWARF:
            case RACE_NIGHTELF:
            case RACE_UNDEAD_PLAYER:
            case RACE_GNOME:
            case RACE_TROLL:
            case RACE_BLOODELF:
            case RACE_DRAENEI:
            {
                pPlayer->ADD_GOSSIP_ITEM(0, "Change Hair Style.", GOSSIP_SENDER_MAIN, 1504);
                pPlayer->ADD_GOSSIP_ITEM(0, "Change Hair Color.", GOSSIP_SENDER_MAIN, 1505);
            }
            break;
            case RACE_TAUREN:
            {
                pPlayer->ADD_GOSSIP_ITEM(0, "Change Horn Style.", GOSSIP_SENDER_MAIN, 1504);
                pPlayer->ADD_GOSSIP_ITEM(0, "Change Horn Color.", GOSSIP_SENDER_MAIN, 1505);
            }
            break;
            }

            switch (pPlayer->getRace())
            {
            case RACE_HUMAN:
            case RACE_ORC:
            case RACE_DWARF:
                pPlayer->getGender() == GENDER_MALE ? pPlayer->ADD_GOSSIP_ITEM(0, "Change Facial Hair.", GOSSIP_SENDER_MAIN, 1506) : pPlayer->ADD_GOSSIP_ITEM(0, "Change Piercings.", GOSSIP_SENDER_MAIN, 1506);
                break;
            case RACE_NIGHTELF:
                pPlayer->getGender() == GENDER_MALE ? pPlayer->ADD_GOSSIP_ITEM(0, "Change Facial Hair.", GOSSIP_SENDER_MAIN, 1506) : pPlayer->ADD_GOSSIP_ITEM(0, "Change Markings.", GOSSIP_SENDER_MAIN, 1506);
                break;
            case RACE_UNDEAD_PLAYER:
                pPlayer->ADD_GOSSIP_ITEM(0, "Change Features.", GOSSIP_SENDER_MAIN, 1506);
                break;
            case RACE_TAUREN:
                pPlayer->getGender() == GENDER_MALE ? pPlayer->ADD_GOSSIP_ITEM(0, "Change Facial Hair.", GOSSIP_SENDER_MAIN, 1506) : pPlayer->ADD_GOSSIP_ITEM(0, "Change Hair.", GOSSIP_SENDER_MAIN, 1506);
                break;
            case RACE_GNOME:
                pPlayer->getGender() == GENDER_MALE ? pPlayer->ADD_GOSSIP_ITEM(0, "Change Facial Hair.", GOSSIP_SENDER_MAIN, 1506) : pPlayer->ADD_GOSSIP_ITEM(0, "Change Earrings.", GOSSIP_SENDER_MAIN, 1506);
                break;
            case RACE_TROLL:
                pPlayer->ADD_GOSSIP_ITEM(0, "Change Tusks.", GOSSIP_SENDER_MAIN, 1506);
                break;
            case RACE_BLOODELF:
                pPlayer->getGender() == GENDER_MALE ? pPlayer->ADD_GOSSIP_ITEM(0, "Change Facial Hair.", GOSSIP_SENDER_MAIN, 1506) : pPlayer->ADD_GOSSIP_ITEM(0, "Change Earrings.", GOSSIP_SENDER_MAIN, 1506);
                break;
            case RACE_DRAENEI:
                pPlayer->getGender() == GENDER_MALE ? pPlayer->ADD_GOSSIP_ITEM(0, "Change Facial Hair.", GOSSIP_SENDER_MAIN, 1506) : pPlayer->ADD_GOSSIP_ITEM(0, "Change Horn Style.", GOSSIP_SENDER_MAIN, 1506);
                break;
            }

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        }
        break;
        case 1003:
        {
            QueryResult* type3 = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 3");
            Field *fields3 = type3->Fetch();
            uint32 gold = fields3[0].GetUInt32();
            uint32 honor = fields3[1].GetUInt32();
            uint32 ap = fields3[2].GetUInt32();
            uint32 itemid = fields3[3].GetUInt32();
            uint32 itemcount = fields3[4].GetUInt32();
            if (pPlayer->GetMoney() < gold * 10000)
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterWhisper("You have not enough Gold", pPlayer->GetGUID());
                return false;
            }
            if (pPlayer->GetHonorPoints() < honor)
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterWhisper("You have not enough Honor Points", pPlayer->GetGUID());
                return false;
            }
            if (pPlayer->GetArenaPoints() < ap)
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterWhisper("You have not enough Arena Points", pPlayer->GetGUID());
                return false;
            }
            if (pPlayer->HasItemCount(itemid, itemcount))
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterWhisper("You have not enough Items", pPlayer->GetGUID());
                return false;
            }

            pPlayer->ModifyMoney((-1)*gold);
            pPlayer->ModifyHonorPoints((-1)*honor);
            pPlayer->ModifyArenaPoints((-1)*ap);
            pPlayer->DestroyItemCount(itemid, itemcount, true);

            pPlayer->SetAtLoginFlag(AT_LOGIN_RENAME);
            pPlayer->GetSession()->LogoutPlayer(true);
        }
        break;
        case 1501: // Gender
        {
            QueryResult* cost = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 3");
            Field *costfields = cost->Fetch();
            pPlayer->ModifyMoney((-1)*costfields[0].GetUInt32());
            pPlayer->ModifyHonorPoints((-1)*costfields[1].GetUInt32());
            pPlayer->ModifyArenaPoints((-1)*costfields[2].GetUInt32());
            pPlayer->DestroyItemCount(costfields[3].GetUInt32(), costfields[4].GetUInt32(), true);

            uint32 displayId = pPlayer->GetNativeDisplayId();
            uint32 new_displayId = displayId;
            Gender gender;

            if (pPlayer->getGender() == GENDER_FEMALE)
            {
                new_displayId = pPlayer->getRace() == RACE_BLOODELF ? displayId + 1 : displayId - 1;
                gender = GENDER_MALE;
            }
            else if (pPlayer->getGender() == GENDER_MALE)
            {
                new_displayId = pPlayer->getRace() == RACE_BLOODELF ? displayId - 1 : displayId + 1;
                gender = GENDER_FEMALE;
            }

            // Set gender
            pPlayer->SetByteValue(UNIT_FIELD_BYTES_0, 2, gender);
            pPlayer->SetByteValue(PLAYER_BYTES_3, 0, gender);

            // Change display ID
            pPlayer->SetDisplayId(new_displayId);
            pPlayer->SetNativeDisplayId(new_displayId);

            if (pPlayer->getGender() == GENDER_FEMALE)
                pCreature->MonsterWhisper("You FEMALE now!!!", pPlayer->GetGUID());
            else
                pCreature->MonsterWhisper("You MALE now!!!", pPlayer->GetGUID());
            pPlayer->CLOSE_GOSSIP_MENU();
        }
        break;
        case 1502: // Skin color
        {
            QueryResult* cost = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 3");
            Field *costfields = cost->Fetch();
            pPlayer->ModifyMoney((-1)*costfields[0].GetUInt32());
            pPlayer->ModifyHonorPoints((-1)*costfields[1].GetUInt32());
            pPlayer->ModifyArenaPoints((-1)*costfields[2].GetUInt32());
            pPlayer->DestroyItemCount(costfields[3].GetUInt32(), costfields[4].GetUInt32(), true);

            pPlayer->ADD_GOSSIP_ITEM(0, "Next Skin Color...", GOSSIP_SENDER_MAIN, 1507);
            pPlayer->ADD_GOSSIP_ITEM(0, "Prev Skin Color...", GOSSIP_SENDER_MAIN, 1508);
            pPlayer->ADD_GOSSIP_ITEM(0, "I Like It...", GOSSIP_SENDER_MAIN, 1999);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        }
        break;
        case 1503: // Face
        {
            QueryResult* cost = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 3");
            Field *costfields = cost->Fetch();
            pPlayer->ModifyMoney((-1)*costfields[0].GetUInt32());
            pPlayer->ModifyHonorPoints((-1)*costfields[1].GetUInt32());
            pPlayer->ModifyArenaPoints((-1)*costfields[2].GetUInt32());
            pPlayer->DestroyItemCount(costfields[3].GetUInt32(), costfields[4].GetUInt32(), true);

            pPlayer->ADD_GOSSIP_ITEM(0, "Next Face...", GOSSIP_SENDER_MAIN, 1509);
            pPlayer->ADD_GOSSIP_ITEM(0, "Prev Face...", GOSSIP_SENDER_MAIN, 1510);
            pPlayer->ADD_GOSSIP_ITEM(0, "I Like It...", GOSSIP_SENDER_MAIN, 1999);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        }
        break;
        case 1504: // Hair style
        {
            QueryResult* cost = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 3");
            Field *costfields = cost->Fetch();
            pPlayer->ModifyMoney((-1)*costfields[0].GetUInt32());
            pPlayer->ModifyHonorPoints((-1)*costfields[1].GetUInt32());
            pPlayer->ModifyArenaPoints((-1)*costfields[2].GetUInt32());
            pPlayer->DestroyItemCount(costfields[3].GetUInt32(), costfields[4].GetUInt32(), true);

            pPlayer->ADD_GOSSIP_ITEM(0, "Next Style...", GOSSIP_SENDER_MAIN, 1511);
            pPlayer->ADD_GOSSIP_ITEM(0, "Prev Style...", GOSSIP_SENDER_MAIN, 1512);
            pPlayer->ADD_GOSSIP_ITEM(0, "I Like It...", GOSSIP_SENDER_MAIN, 1999);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        }
        break;
        case 1505: // Hair color
        {
            QueryResult* cost = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 3");
            Field *costfields = cost->Fetch();
            pPlayer->ModifyMoney((-1)*costfields[0].GetUInt32());
            pPlayer->ModifyHonorPoints((-1)*costfields[1].GetUInt32());
            pPlayer->ModifyArenaPoints((-1)*costfields[2].GetUInt32());
            pPlayer->DestroyItemCount(costfields[3].GetUInt32(), costfields[4].GetUInt32(), true);

            pPlayer->ADD_GOSSIP_ITEM(0, "Next Color...", GOSSIP_SENDER_MAIN, 1513);
            pPlayer->ADD_GOSSIP_ITEM(0, "Prev Color...", GOSSIP_SENDER_MAIN, 1514);
            pPlayer->ADD_GOSSIP_ITEM(0, "I Like It...", GOSSIP_SENDER_MAIN, 1999);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        }
        break;
        case 1506: // Accessuares
        {
            QueryResult* cost = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 3");
            Field *costfields = cost->Fetch();
            pPlayer->ModifyMoney((-1)*costfields[0].GetUInt32());
            pPlayer->ModifyHonorPoints((-1)*costfields[1].GetUInt32());
            pPlayer->ModifyArenaPoints((-1)*costfields[2].GetUInt32());
            pPlayer->DestroyItemCount(costfields[3].GetUInt32(), costfields[4].GetUInt32(), true);

            pPlayer->ADD_GOSSIP_ITEM(0, "Next Style...", GOSSIP_SENDER_MAIN, 1515);
            pPlayer->ADD_GOSSIP_ITEM(0, "Prev Style...", GOSSIP_SENDER_MAIN, 1516);
            pPlayer->ADD_GOSSIP_ITEM(0, "I Like It...", GOSSIP_SENDER_MAIN, 1999);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        }
        break;
        case 1507: // Next skin
        {
            SelectSkin(pPlayer, 1);
            pPlayer->ADD_GOSSIP_ITEM(0, "Next Skin Color...", GOSSIP_SENDER_MAIN, 1507);
            pPlayer->ADD_GOSSIP_ITEM(0, "Prev Skin Color...", GOSSIP_SENDER_MAIN, 1508);
            pPlayer->ADD_GOSSIP_ITEM(0, "I Like It...", GOSSIP_SENDER_MAIN, 1999);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        }
        break;
        case 1508: // Prev skin
        {
            SelectSkin(pPlayer, -1);
            pPlayer->ADD_GOSSIP_ITEM(0, "Next Skin Color...", GOSSIP_SENDER_MAIN, 1507);
            pPlayer->ADD_GOSSIP_ITEM(0, "Prev Skin Color...", GOSSIP_SENDER_MAIN, 1508);
            pPlayer->ADD_GOSSIP_ITEM(0, "I Like It...", GOSSIP_SENDER_MAIN, 1999);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        }
        break;
        case 1509: // Next face
        {
            SelectFace(pPlayer, 1);
            pPlayer->ADD_GOSSIP_ITEM(0, "Next Skin Color...", GOSSIP_SENDER_MAIN, 1509);
            pPlayer->ADD_GOSSIP_ITEM(0, "Prev Skin Color...", GOSSIP_SENDER_MAIN, 1510);
            pPlayer->ADD_GOSSIP_ITEM(0, "I Like It...", GOSSIP_SENDER_MAIN, 1999);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        }
        break;
        case 1510: // Prev face
        {
            SelectFace(pPlayer, -1);
            pPlayer->ADD_GOSSIP_ITEM(0, "Next Skin Color...", GOSSIP_SENDER_MAIN, 1509);
            pPlayer->ADD_GOSSIP_ITEM(0, "Prev Skin Color...", GOSSIP_SENDER_MAIN, 1510);
            pPlayer->ADD_GOSSIP_ITEM(0, "I Like It...", GOSSIP_SENDER_MAIN, 1999);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        }
        break;
        case 1511: // Next hair style
        {
            SelectHairStyle(pPlayer, 1);
            pPlayer->ADD_GOSSIP_ITEM(0, "Next Skin Color...", GOSSIP_SENDER_MAIN, 1511);
            pPlayer->ADD_GOSSIP_ITEM(0, "Prev Skin Color...", GOSSIP_SENDER_MAIN, 1512);
            pPlayer->ADD_GOSSIP_ITEM(0, "I Like It...", GOSSIP_SENDER_MAIN, 1999);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        }
        break;
        case 1512: // Prev hair style
        {
            SelectHairStyle(pPlayer, -1);
            pPlayer->ADD_GOSSIP_ITEM(0, "Next Skin Color...", GOSSIP_SENDER_MAIN, 1511);
            pPlayer->ADD_GOSSIP_ITEM(0, "Prev Skin Color...", GOSSIP_SENDER_MAIN, 1512);
            pPlayer->ADD_GOSSIP_ITEM(0, "I Like It...", GOSSIP_SENDER_MAIN, 1999);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        }
        break;
        case 1513: // Next hair color
        {
            SelectHairColor(pPlayer, 1);
            pPlayer->ADD_GOSSIP_ITEM(0, "Next Skin Color...", GOSSIP_SENDER_MAIN, 1513);
            pPlayer->ADD_GOSSIP_ITEM(0, "Prev Skin Color...", GOSSIP_SENDER_MAIN, 1514);
            pPlayer->ADD_GOSSIP_ITEM(0, "I Like It...", GOSSIP_SENDER_MAIN, 1999);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        }
        break;
        case 1514: // Prev hair color
        {
            SelectHairColor(pPlayer, -1);
            pPlayer->ADD_GOSSIP_ITEM(0, "Next Skin Color...", GOSSIP_SENDER_MAIN, 1513);
            pPlayer->ADD_GOSSIP_ITEM(0, "Prev Skin Color...", GOSSIP_SENDER_MAIN, 1514);
            pPlayer->ADD_GOSSIP_ITEM(0, "I Like It...", GOSSIP_SENDER_MAIN, 1999);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        }
        break;
        case 1515: // Next style
        {
            SelectFacialFeature(pPlayer, 1);
            pPlayer->ADD_GOSSIP_ITEM(0, "Next Skin Color...", GOSSIP_SENDER_MAIN, 1515);
            pPlayer->ADD_GOSSIP_ITEM(0, "Prev Skin Color...", GOSSIP_SENDER_MAIN, 1516);
            pPlayer->ADD_GOSSIP_ITEM(0, "I Like It...", GOSSIP_SENDER_MAIN, 1999);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        }
        break;
        case 1516: // Prev style
        {
            SelectFacialFeature(pPlayer, -1);
            pPlayer->ADD_GOSSIP_ITEM(0, "Next Skin Color...", GOSSIP_SENDER_MAIN, 1515);
            pPlayer->ADD_GOSSIP_ITEM(0, "Prev Skin Color...", GOSSIP_SENDER_MAIN, 1516);
            pPlayer->ADD_GOSSIP_ITEM(0, "I Like It...", GOSSIP_SENDER_MAIN, 1999);

            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        }
        break;
        case 1999:
        {
            pCreature->MonsterWhisper("Complite!!!", pPlayer->GetGUID());
            pPlayer->CLOSE_GOSSIP_MENU();
        }
        break;
        case 2001:
        {
            QueryResult* type1 = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 1");
            Field *fields1 = type1->Fetch();
            pPlayer->ModifyMoney((-1)*fields1[0].GetUInt32());
            pPlayer->ModifyHonorPoints((-1)*fields1[1].GetUInt32());
            pPlayer->ModifyArenaPoints((-1)*fields1[2].GetUInt32());
            pPlayer->DestroyItemCount(fields1[3].GetUInt32(), fields1[4].GetUInt32(), true);

            pPlayer->CastSpell(pPlayer, 24222, true);
            SetRace(pPlayer, pPlayer->getRace(), 1);

        }
        break;
        case 2002:
        {
            QueryResult* type1 = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 1");
            Field *fields1 = type1->Fetch();
            pPlayer->ModifyMoney((-1)*fields1[0].GetUInt32());
            pPlayer->ModifyHonorPoints((-1)*fields1[1].GetUInt32());
            pPlayer->ModifyArenaPoints((-1)*fields1[2].GetUInt32());
            pPlayer->DestroyItemCount(fields1[3].GetUInt32(), fields1[4].GetUInt32(), true);

            pPlayer->CastSpell(pPlayer, 24222, true);
            SetRace(pPlayer, pPlayer->getRace(), 2);
        }
        break;
        case 2003:
        {
            QueryResult* type1 = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 1");
            Field *fields1 = type1->Fetch();
            pPlayer->ModifyMoney((-1)*fields1[0].GetUInt32());
            pPlayer->ModifyHonorPoints((-1)*fields1[1].GetUInt32());
            pPlayer->ModifyArenaPoints((-1)*fields1[2].GetUInt32());
            pPlayer->DestroyItemCount(fields1[3].GetUInt32(), fields1[4].GetUInt32(), true);

            pPlayer->CastSpell(pPlayer, 24222, true);
            SetRace(pPlayer, pPlayer->getRace(), 3);

        }
        break;
        case 2004:
        {
            QueryResult* type1 = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 1");
            Field *fields1 = type1->Fetch();
            pPlayer->ModifyMoney((-1)*fields1[0].GetUInt32());
            pPlayer->ModifyHonorPoints((-1)*fields1[1].GetUInt32());
            pPlayer->ModifyArenaPoints((-1)*fields1[2].GetUInt32());
            pPlayer->DestroyItemCount(fields1[3].GetUInt32(), fields1[4].GetUInt32(), true);

            pPlayer->CastSpell(pPlayer, 24222, true);
            SetRace(pPlayer, pPlayer->getRace(), 4);

        }
        break;
        case 2005:
        {
            QueryResult* type1 = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 1");
            Field *fields1 = type1->Fetch();
            pPlayer->ModifyMoney((-1)*fields1[0].GetUInt32());
            pPlayer->ModifyHonorPoints((-1)*fields1[1].GetUInt32());
            pPlayer->ModifyArenaPoints((-1)*fields1[2].GetUInt32());
            pPlayer->DestroyItemCount(fields1[3].GetUInt32(), fields1[4].GetUInt32(), true);

            pPlayer->CastSpell(pPlayer, 24222, true);
            SetRace(pPlayer, pPlayer->getRace(), 5);

        }
        break;
        case 2006:
        {
            QueryResult* type1 = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 1");
            Field *fields1 = type1->Fetch();
            pPlayer->ModifyMoney((-1)*fields1[0].GetUInt32());
            pPlayer->ModifyHonorPoints((-1)*fields1[1].GetUInt32());
            pPlayer->ModifyArenaPoints((-1)*fields1[2].GetUInt32());
            pPlayer->DestroyItemCount(fields1[3].GetUInt32(), fields1[4].GetUInt32(), true);

            pPlayer->CastSpell(pPlayer, 24222, true);
            SetRace(pPlayer, pPlayer->getRace(), 6);

        }
        break;
        case 2007:
        {
            QueryResult* type1 = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 1");
            Field *fields1 = type1->Fetch();
            pPlayer->ModifyMoney((-1)*fields1[0].GetUInt32());
            pPlayer->ModifyHonorPoints((-1)*fields1[1].GetUInt32());
            pPlayer->ModifyArenaPoints((-1)*fields1[2].GetUInt32());
            pPlayer->DestroyItemCount(fields1[3].GetUInt32(), fields1[4].GetUInt32(), true);

            pPlayer->CastSpell(pPlayer, 24222, true);
            SetRace(pPlayer, pPlayer->getRace(), 7);

        }
        break;
        case 2008:
        {
            QueryResult* type1 = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 1");
            Field *fields1 = type1->Fetch();
            pPlayer->ModifyMoney((-1)*fields1[0].GetUInt32());
            pPlayer->ModifyHonorPoints((-1)*fields1[1].GetUInt32());
            pPlayer->ModifyArenaPoints((-1)*fields1[2].GetUInt32());
            pPlayer->DestroyItemCount(fields1[3].GetUInt32(), fields1[4].GetUInt32(), true);

            pPlayer->CastSpell(pPlayer, 24222, true);
            SetRace(pPlayer, pPlayer->getRace(), 8);

        }
        break;
        case 2009:
        {
            QueryResult* type1 = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 1");
            Field *fields1 = type1->Fetch();
            pPlayer->ModifyMoney((-1)*fields1[0].GetUInt32());
            pPlayer->ModifyHonorPoints((-1)*fields1[1].GetUInt32());
            pPlayer->ModifyArenaPoints((-1)*fields1[2].GetUInt32());
            pPlayer->DestroyItemCount(fields1[3].GetUInt32(), fields1[4].GetUInt32(), true);

            pPlayer->CastSpell(pPlayer, 24222, true);
            SetRace(pPlayer, pPlayer->getRace(), 10);

        }
        break;
        case 2010:
        {
            QueryResult* type1 = WorldDatabase.PQuery("SELECT `cost_gold`, `cost_honor`, `cost_ap`, `cost_itemid`, `constitemcount` FROM `npc_customer` WHERE `type` = 1");
            Field *fields1 = type1->Fetch();
            pPlayer->ModifyMoney((-1)*fields1[0].GetUInt32());
            pPlayer->ModifyHonorPoints((-1)*fields1[1].GetUInt32());
            pPlayer->ModifyArenaPoints((-1)*fields1[2].GetUInt32());
            pPlayer->DestroyItemCount(fields1[3].GetUInt32(), fields1[4].GetUInt32(), true);

            pPlayer->CastSpell(pPlayer, 24222, true);
            SetRace(pPlayer, pPlayer->getRace(), 11);

        }
        break;
        }
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {

        // Show menu
        if (uiSender == GOSSIP_SENDER_MAIN)
            SendDefaultMenu(pPlayer, pCreature, uiAction);
        return true;

    }

};

void Addmod_customizeScripts()
{
    new npc_customizer();
}