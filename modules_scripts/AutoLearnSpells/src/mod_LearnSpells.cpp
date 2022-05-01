#include "ScriptMgr.h"
#include "Player.h"
#include <unordered_set>
#include "World/World.h"

std::vector<uint32>CompareSpell;

std::unordered_set<uint32> m_ignoreSpells =
{
    11, 64380, 23885, 23880, 44461, 25346, 10274, 10273, 8418,  8419,  7270,  7269,  7268,  54648, 12536, 24530, 70909, 12494, 57933, 24224, 27095, 27096, 27097, 27099, 32841, 56131, 56160, 56161, 48153, 34754, 64844, 64904, 48085, 33110, 48084,
    28276, 27874, 27873, 7001,  49821, 53022, 47757, 47750, 47758, 47666, 53001, 52983, 52998, 52986, 52987, 52999, 52984, 53002, 53003, 53000, 52988, 52985, 42208, 42209, 42210, 42211, 42212, 42213, 42198, 42937, 42938, 12484, 12485, 12486,
    44461, 55361, 55362, 34913, 43043, 43044, 38703, 38700, 27076, 42844, 42845, 64891, 25912, 25914, 25911, 25913, 25902, 25903, 27175, 27176, 33073, 33074, 48822, 48820, 48823, 48821, 20154, 25997, 20467, 20425, 67,    26017, 34471, 53254,
    13812, 14314, 14315, 27026, 49064, 49065, 60202, 60210, 13797, 14298, 14299, 14300, 14301, 27024, 49053, 49054, 52399, 1742,  24453, 53548, 53562, 52016, 26064, 35346, 57386, 57389, 57390, 57391, 57392, 57393, 55509, 35886, 43339, 45297,
    45298, 45299, 45300, 45301, 45302, 49268, 49269, 8349,  8502,  8503,  11306, 11307, 25535, 25537, 61650, 61654, 63685, 45284, 45286, 45287, 45288, 45289, 45290, 45291, 45292, 45293, 45294, 45295, 45296, 49239, 49240, 26364, 26365, 26366,
    26367, 26369, 26370, 26363, 26371, 26372, 49278, 49279, 32176, 32175, 21169, 47206, 27285, 47833, 47836, 42223, 42224, 42225, 42226, 42218, 47817, 47818, 42231, 42232, 42233, 42230, 48466, 44203, 44205, 44206, 44207, 44208, 48444, 48445,
    33891, 52374, 57532, 59921, 52372, 49142, 52375, 47633, 47632, 52373, 50536, 27214, 47822, 11682, 11681, 5857,  1010,  24907, 24905, 53227, 61391, 61390, 61388, 61387, 64801, 5421,  9635,  1178,  20186, 20185, 20184, 20187, 25899, 24406,
    50581, 30708, 48076, 62900, 62901, 62902, 59671, 50589, 66906, 66907, 24131, 23455, 23458, 23459, 27803, 27804, 27805, 25329, 48075, 42243, 42244, 42245, 42234, 58432, 58433, 65878, 18848, 16979, 49376, 54055, 20647, 42243, 24131, 34936,
    31117, 701, 3110, 29834, 30029, 12966,
    // COSMETIC SPELLS
    28271, 28272, 61025, 61305, 61721, 61780,
    // OPTIONAL QUEST SPELLS
    18540,
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

    bool IsIgnoredSpell(uint32 SpellID)
    {
        return m_ignoreSpells.find(SpellID) != m_ignoreSpells.end();
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

            // Is this spellid in ignored spell list?
            if (IsIgnoredSpell(spellInfo->Id))
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
    LoadSpellTable() : WorldScript("load_spellTable") { }

    void OnLoadCustomDatabaseTable()
    {
        sLog.outString("Loading Spell Table...");

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

        sLog.outString(">> Loaded %u Spells in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        sLog.outString("");
    }
};

void Addmod_LearnSpellsScripts()
{
    new LoadSpellTable();
    new LearnSpellsOnLevelUp();
}