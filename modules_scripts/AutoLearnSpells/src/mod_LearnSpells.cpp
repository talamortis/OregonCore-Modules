#include "ScriptMgr.h"
#include "Player.h"
#include <unordered_set>
#include "World/World.h"

std::vector<uint32>CompareSpell;

std::vector<uint32>ForcedSpell = {
// Druid
    5487, // Bear form
    6795, // Growl
    6807, // Maul
// Warrior
    71,   // Defence Stance
    355,  // Taunt
    7386, // Sunder Armor
    2458, // Berserker Stance
    20252,// Intercept
// Warlock
    688,  // Summon Imp
    697,  // Summon Voidwalker
    712,  // Summon Succubus
    691,  // Summon Felhunter
// Paladin
    7328, // Redemption
// Shaman
    8071, // Stoneskin Totem
    3599, // Searing Totem
    5394, // Healing Stream Totem
// Hunter
    5149, // Beast Training
    1515, // Tame Beast
    883,  // Call Pet
    982,  // Revive Pet
    6991, // Feed Pet
};
class LearnSpellsOnLevelUp : public PlayerScript
{
public:
    LearnSpellsOnLevelUp() : PlayerScript("LearnSpellsOnLevelUp") { }

    void OnLogin(Player* player, bool firstLogin)
    {
        if (firstLogin)
            LearnSpellsForNewLevel(player, 1);
    }

    void OnLevelChanged(Player* player, uint8 oldLevel, uint8 newLevel) override
    {
        LearnSpellsForNewLevel(player, newLevel);
    }

    void LearnSpellsForNewLevel(Player* player, uint32 newLevel)
    {
        ChrClassesEntry const* clsEntry = sChrClassesStore.LookupEntry(player->getClass());
        if (!clsEntry)
            return;

        uint32 family = clsEntry->spellfamily;

        for (uint32 i = 0; i < sSkillLineAbilityStore.GetNumRows(); ++i)
        {
            SkillLineAbilityEntry const* entry = sSkillLineAbilityStore.LookupEntry(i);
            if (!entry)
                continue;

            SpellEntry const* spellInfo = sSpellStore.LookupEntry(entry->spellId);
            if (!spellInfo)
                continue;

            // skip server-side/triggered spells
            if (spellInfo->spellLevel == 0)
                continue;
            
            // Check players new level to spell level
            if (spellInfo->spellLevel != newLevel)
                continue;

            // skip focus spells
            if (spellInfo->powerType == POWER_FOCUS)
                continue;

            // skip wrong class/race skills
            if (!player->IsSpellFitByClassAndRace(spellInfo->Id))
                continue;

            // skip other spell families
            if (spellInfo->SpellFamilyName != family)
                continue;

            // if we already have the spell then skip
            if (player->HasSpell(spellInfo->Id))
                continue;

           // Dont Add Talent point spells
            TalentEntry const* talentEntry = sTalentStore.LookupEntry(spellInfo->Id);
            if (talentEntry)
                continue;

            // skip spells with first rank learned as talent (and all talents then also)
            uint32 first_rank = sSpellMgr.GetFirstSpellInChain(spellInfo->Id);
            if (GetTalentSpellCost(first_rank) > 0)
                continue;

            // skip broken spells
            if (!SpellMgr::IsSpellValid(spellInfo, player, false))
                continue;

            // check table to make sure player can use that spell
            std::vector<uint32>::iterator it;
            it = std::find(CompareSpell.begin(), CompareSpell.end(), spellInfo->Id);

            if (it == CompareSpell.end())
                continue;

            player->LearnSpell(spellInfo->Id);
        }
    }
};


class LoadSpellTable : public WorldScript
{
public:
    LoadSpellTable() : WorldScript("load_AutoLearnSpells") { }

    void OnLoadCustomDatabaseTable()
    {
        sLog.outString("Loading Spells for AutoLearnSpells Table...");

        QueryResult* result = WorldDatabase.PQuery("SELECT `spell` FROM npc_trainer");

        if (!result)
        {
            sLog.outErrorDb(">>  Loaded 0 npc_trainer. DB table `npc_trainer` is empty!");
            sLog.outString();
            return;
        }

        uint32 count = 0;
        uint32 oldMSTime = getMSTime();

        do
        {
            Field* field = result->Fetch();
            CompareSpell.push_back(field[0].GetUInt32());
            count++;

        } while (result->NextRow());

        //Add Missing spells here
        CompareSpell.insert(CompareSpell.end(), std::make_move_iterator(ForcedSpell.begin()),
            std::make_move_iterator(ForcedSpell.end()));

        sLog.outString(">> Loaded %u Spells for AutoLearnSpells in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        sLog.outString("");

    }
};

void Addmod_LearnSpellsScripts()
{
    new LoadSpellTable();
    new LearnSpellsOnLevelUp();
}