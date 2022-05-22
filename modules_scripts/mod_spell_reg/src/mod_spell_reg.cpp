#include "ScriptMgr.h"
#include "Player.h"
#include <unordered_set>
#include "World/World.h"
#include "SpellMgr.h"
#include <string> 

struct SpellBonusEntry
{
    float  direct_damage;
    float  dot_damage;
    float  ap_bonus;
    float  dot_ap_bonus;
    float  direct_damage_player;
    float  dot_damage_player;
    float  ap_bonus_player;
    float  dot_ap_bonus_player;
};

typedef UNORDERED_MAP<uint32, SpellBonusEntry> SpellBonusMap;
SpellBonusMap mSpellBonusMap;

class mod_spell_reg : public PlayerScript
{
public:
    mod_spell_reg() : PlayerScript("mod_spell_reg") { }

    void OnLogin(Player* player, bool firstLogin)
    {
        ChatHandler(player).PSendSysMessage("This server is using Spell Mods");
    }
};

// Spell bonus data
SpellBonusEntry const* GetSpellBonusData(uint32 spellId)
{
    // Lookup data
    SpellBonusMap::const_iterator itr = mSpellBonusMap.find(spellId);
    if (itr != mSpellBonusMap.end())
        return &itr->second;
    // Not found, try lookup for 1 spell rank if exist
    if (uint32 rank_1 = sSpellMgr.GetFirstSpellInChain(spellId))
    {
        SpellBonusMap::const_iterator itr = mSpellBonusMap.find(rank_1);
        if (itr != mSpellBonusMap.end())
            return &itr->second;
    }
    return NULL;
}

class spell_reg_unit : public UnitScript
{

public:
    spell_reg_unit() : UnitScript("spell_reg_unit") {}

    void OnGetSpellDamageBonus(Unit* caster, Unit* victim, SpellEntry const* spellProto, float& pdamage, DamageEffectType damagetype)
    {
        if (!spellProto)
            return;

        if (caster->GetTypeId() != TYPEID_PLAYER)
            return;
        
        if (!GetSpellBonusData(spellProto->Id))
            return;

        if (victim->GetTypeId() != TYPEID_PLAYER) // Creature 
        {
            if (damagetype == DOT && spellProto->DmgClass == SPELL_DAMAGE_CLASS_MELEE)
                pdamage *= GetSpellBonusData(spellProto->Id)->dot_ap_bonus;
            else if (damagetype == DOT && spellProto->DmgClass == SPELL_DAMAGE_CLASS_MAGIC)
                pdamage *= GetSpellBonusData(spellProto->Id)->dot_damage;
            else if (damagetype == SPELL_DIRECT_DAMAGE)
                pdamage *= GetSpellBonusData(spellProto->Id)->direct_damage;
        }
        else // Player
        {
            if (damagetype == DOT && spellProto->DmgClass == SPELL_DAMAGE_CLASS_MELEE)
                pdamage *= GetSpellBonusData(spellProto->Id)->dot_ap_bonus_player;
            else if (damagetype == DOT && spellProto->DmgClass == SPELL_DAMAGE_CLASS_MAGIC)
                pdamage *= GetSpellBonusData(spellProto->Id)->dot_damage_player;
            else if (damagetype == SPELL_DIRECT_DAMAGE)
                pdamage *= GetSpellBonusData(spellProto->Id)->direct_damage_player;
        }

    }
    

    void OnGetSpellHealingBonus(Unit* caster, SpellEntry const* spellProto, float& healamount, DamageEffectType damagetype, Unit* victim)
    {
        if (!spellProto)
            return;

        if (caster->GetTypeId() != TYPEID_PLAYER)
            return;

        if (!GetSpellBonusData(spellProto->Id))
            return;

        // Creature
        if (victim->GetTypeId() != TYPEID_PLAYER)
        {
            if (damagetype == DOT)
                healamount *= GetSpellBonusData(spellProto->Id)->dot_damage;
            else if (damagetype == HEAL)
                healamount *= GetSpellBonusData(spellProto->Id)->direct_damage;
        }
        else // Player
        {
            if (damagetype == DOT)
                healamount *= GetSpellBonusData(spellProto->Id)->dot_damage_player;
            else if (damagetype == HEAL)
                healamount *= GetSpellBonusData(spellProto->Id)->direct_damage_player;
        }

    }

    void OnGetMeleeDamageBonus(Unit* caster, Unit* victim, float& damage, WeaponAttackType attType, SpellEntry const* spellProto)
    {
        if (!spellProto)
            return;

        if (caster->GetTypeId() != TYPEID_PLAYER)
            return;

        if (!GetSpellBonusData(spellProto->Id))
            return;

        if (victim->GetTypeId() != TYPEID_PLAYER) // Creature
            damage *= GetSpellBonusData(spellProto->Id)->ap_bonus;
        else // Player
            damage *= GetSpellBonusData(spellProto->Id)->ap_bonus_player;
    }

    void OnModifyAura(Unit* caster, Aura* aura, Modifier*& mod)
    {
        if (caster->GetTypeId() != TYPEID_PLAYER)
            return;

        if (!GetSpellBonusData(aura->GetId()))
            return;

        if (!aura->IsPositive())
            return;

        // make sure we are not healing spell or damage
        SpellEntry const* spellProto = aura->GetSpellProto();
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; i++)
        {
            if (spellProto->EffectApplyAuraName[i] == SPELL_AURA_PERIODIC_HEAL || spellProto->EffectApplyAuraName[i] == SPELL_AURA_MOD_DAMAGE_DONE || spellProto->EffectApplyAuraName[i] == SPELL_AURA_MOD_HEALING)
                return;
        }

        mod->m_amount *= GetSpellBonusData(aura->GetId())->direct_damage;
    }
};


class LoadSpellModTable : public WorldScript
{
public:
    LoadSpellModTable() : WorldScript("load_spell_mods") { }

    void OnLoadCustomDatabaseTable()
    {
        uint32 oldMSTime = getMSTime();

        mSpellBonusMap.clear();                             // need for reload case

        //                                                0      1             2          3         4               5                   6                   7               8
        QueryResult_AutoPtr result = WorldDatabase.Query("SELECT entry, direct_bonus, dot_bonus, ap_bonus, dot_ap_bonus, direct_bonus_player, dot_bonus_player, ap_bonus_player, dot_ap_bonus_player FROM spell_bonus_data");
        if (!result)
        {
            sLog.outString("server.loading", ">> Loaded 0 spell bonus data. DB table `spell_bonus_data` is empty.");
            sLog.outString("server.loading", " ");
            return;
        }

        uint32 count = 0;
        do
        {
            Field* fields = result->Fetch();
            uint32 entry = fields[0].GetUInt32();

            const SpellEntry* spell = sSpellStore.LookupEntry(entry);
            if (!spell)
            {
                sLog.outString("sql.sql", "Spell {} listed in `spell_bonus_data` does not exist", entry);
                continue;
            }

            SpellBonusEntry& sbe = mSpellBonusMap[entry];
            sbe.direct_damage = fields[1].GetFloat();
            sbe.dot_damage = fields[2].GetFloat();
            sbe.ap_bonus = fields[3].GetFloat();
            sbe.dot_ap_bonus = fields[4].GetFloat();
            sbe.direct_damage_player = fields[5].GetFloat();
            sbe.dot_damage_player = fields[6].GetFloat();
            sbe.ap_bonus_player = fields[7].GetFloat();
            sbe.dot_ap_bonus_player = fields[8].GetFloat();

            ++count;
        } while (result->NextRow());

        sLog.outString("server.loading", ">> Loaded {} extra spell bonus data in {} ms", count, GetMSTimeDiffToNow(oldMSTime));
        sLog.outString("server.loading", " ");
    }
};

class cs_spell_bonus_data_reload : public CommandScript
{
public:
    cs_spell_bonus_data_reload() : CommandScript("cs_spell_bonus_data_reload") { }

    static bool HandleReloadSpellData(ChatHandler* pChat, const char* msg)
    {
        Player* me = pChat->GetSession()->GetPlayer();
        uint32 oldMSTime = getMSTime();

        mSpellBonusMap.clear();                             // need for reload case

        //                                                0      1             2          3         4               5                   6                   7               8
        QueryResult_AutoPtr result = WorldDatabase.Query("SELECT entry, direct_bonus, dot_bonus, ap_bonus, dot_ap_bonus, direct_bonus_player, dot_bonus_player, ap_bonus_player, dot_ap_bonus_player FROM spell_bonus_data");

        uint32 count = 0;
        do
        {
            Field* fields = result->Fetch();
            uint32 entry = fields[0].GetUInt32();

            const SpellEntry* spell = sSpellStore.LookupEntry(entry);
            if (!spell)
            {
                sLog.outString("sql.sql", "Spell {} listed in `spell_bonus_data` does not exist", entry);
                continue;
            }

            SpellBonusEntry& sbe = mSpellBonusMap[entry];
            sbe.direct_damage = fields[1].GetFloat();
            sbe.dot_damage = fields[2].GetFloat();
            sbe.ap_bonus = fields[3].GetFloat();
            sbe.dot_ap_bonus = fields[4].GetFloat();
            sbe.direct_damage_player = fields[5].GetFloat();
            sbe.dot_damage_player = fields[6].GetFloat();
            sbe.ap_bonus_player = fields[7].GetFloat();
            sbe.dot_ap_bonus_player = fields[8].GetFloat();

            ++count;
        } while (result->NextRow());

        ChatHandler(me->GetSession()).PSendSysMessage("Reloaded %u spell_bonus in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

        return true;
    }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> spell_bonus_command =
        {
            { "reload",      SEC_ADMINISTRATOR,     false,    &HandleReloadSpellData,      ""},
        };

        static std::vector<ChatCommand> cmdtable =
        {
            { "spell_bonus", SEC_ADMINISTRATOR, true, NULL , "", spell_bonus_command},
        };

        return cmdtable;
    }
};

void Addmod_spell_regScripts()
{
    new LoadSpellModTable();
    new mod_spell_reg();
    new spell_reg_unit();
    new cs_spell_bonus_data_reload();
}